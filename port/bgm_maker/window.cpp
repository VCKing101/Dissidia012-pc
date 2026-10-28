// The Battle BGM Maker's window; see window.hpp. The Vulkan set-up is the
// smallest the ImGui back end needs: one graphics queue that presents, a
// swapchain made and resized by the back end's ImGui_ImplVulkanH helpers.
// MIT licence; see port/README.md.

#include "window.hpp"

#include "input_script.hpp"
#include "theme.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_vulkan.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <array>
#include <cstring>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

namespace d012::bgm_maker {

struct Window::Vulkan {
    VkInstance instance{};
    VkPhysicalDevice physical{};
    VkDevice device{};
    std::uint32_t queue_family{};
    VkQueue queue{};
    VkSurfaceKHR surface{};
    ImGui_ImplVulkanH_Window wd;
    std::uint32_t min_image_count{2};
    VkImageUsageFlags extra_usage{};  // transfer source, for the capture switch, where the surface allows it
    // A capture's copy of the frame.
    VkBuffer capture_buffer{};
    VkDeviceMemory capture_memory{};
    int capture_width{};
    int capture_height{};
};

namespace {

constexpr int kWidth = 1280;
constexpr int kHeight = 800;
// After input, a few more frames, so ImGui settles (hover, animations).
constexpr int kSettleFrames = 3;

void check(VkResult result) {
    if (result == VK_SUCCESS) return;
    std::cerr << "[bgm-maker] Vulkan error " << static_cast<int>(result) << "\n";
}

bool system_dark() { return SDL_GetSystemTheme() == SDL_SYSTEM_THEME_DARK; }

} // namespace

Window::~Window() { close(); }

bool Window::open(const std::string &title, std::string &error) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        error = std::string("SDL cannot start: ") + SDL_GetError();
        return false;
    }
    scale_ = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    if (scale_ <= 0.0f) scale_ = 1.0f;
    window_ = SDL_CreateWindow(title.c_str(), static_cast<int>(kWidth * scale_), static_cast<int>(kHeight * scale_),
                               SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_HIDDEN);
    if (window_ == nullptr) {
        error = std::string("the window cannot be opened: ") + SDL_GetError();
        return false;
    }
    SDL_SetWindowMinimumSize(window_, static_cast<int>(800 * scale_), static_cast<int>(520 * scale_));
    if (!init_vulkan(error)) return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    imgui_sdl_ = ImGui_ImplSDL3_InitForVulkan(window_);
    if (!imgui_sdl_) {
        error = "ImGui's SDL3 back end cannot start";
        return false;
    }
    ImGui_ImplVulkan_InitInfo info{};
    info.ApiVersion = VK_API_VERSION_1_1;
    info.Instance = vk_->instance;
    info.PhysicalDevice = vk_->physical;
    info.Device = vk_->device;
    info.QueueFamily = vk_->queue_family;
    info.Queue = vk_->queue;
    info.DescriptorPoolSize = IMGUI_IMPL_VULKAN_MINIMUM_SAMPLED_IMAGE_POOL_SIZE + 8;
    info.MinImageCount = vk_->min_image_count;
    info.ImageCount = vk_->wd.ImageCount;
    info.PipelineInfoMain.RenderPass = vk_->wd.RenderPass;
    info.PipelineInfoMain.Subpass = 0;
    info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    info.CheckVkResultFn = check;
    imgui_vulkan_ = ImGui_ImplVulkan_Init(&info);
    if (!imgui_vulkan_) {
        error = "ImGui's Vulkan back end cannot start";
        return false;
    }
    theme::load_fonts();
    dark_ = look_dark();
    apply_look();
    SDL_ShowWindow(window_);
    return true;
}

bool Window::init_vulkan(std::string &error) {
    vk_ = new Vulkan();
    std::uint32_t extension_count = 0;
    const char *const *extensions = SDL_Vulkan_GetInstanceExtensions(&extension_count);
    if (extensions == nullptr) {
        error = std::string("no Vulkan: ") + SDL_GetError();
        return false;
    }
    VkApplicationInfo app{VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app.pApplicationName = "Battle BGM Maker";
    app.apiVersion = VK_API_VERSION_1_1;
    VkInstanceCreateInfo create{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    create.pApplicationInfo = &app;
    create.enabledExtensionCount = extension_count;
    create.ppEnabledExtensionNames = extensions;
    if (vkCreateInstance(&create, nullptr, &vk_->instance) != VK_SUCCESS) {
        error = "Vulkan cannot start (is the graphics driver up to date?)";
        return false;
    }
    if (!SDL_Vulkan_CreateSurface(window_, vk_->instance, nullptr, &vk_->surface)) {
        error = std::string("no Vulkan surface: ") + SDL_GetError();
        return false;
    }
    vk_->physical = ImGui_ImplVulkanH_SelectPhysicalDevice(vk_->instance);
    if (vk_->physical == VK_NULL_HANDLE) {
        error = "no graphics device with Vulkan";
        return false;
    }
    vk_->queue_family = ImGui_ImplVulkanH_SelectQueueFamilyIndex(vk_->physical);
    VkBool32 presents = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(vk_->physical, vk_->queue_family, vk_->surface, &presents);
    if (vk_->queue_family == static_cast<std::uint32_t>(-1) || presents != VK_TRUE) {
        error = "the graphics device cannot show this window";
        return false;
    }
    const float priority = 1.0f;
    VkDeviceQueueCreateInfo queue{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
    queue.queueFamilyIndex = vk_->queue_family;
    queue.queueCount = 1;
    queue.pQueuePriorities = &priority;
    const char *device_extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    VkDeviceCreateInfo device{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    device.queueCreateInfoCount = 1;
    device.pQueueCreateInfos = &queue;
    device.enabledExtensionCount = 1;
    device.ppEnabledExtensionNames = device_extensions;
    if (vkCreateDevice(vk_->physical, &device, nullptr, &vk_->device) != VK_SUCCESS) {
        error = "the graphics device cannot be opened";
        return false;
    }
    vkGetDeviceQueue(vk_->device, vk_->queue_family, 0, &vk_->queue);

    ImGui_ImplVulkanH_Window &wd = vk_->wd;
    wd.Surface = vk_->surface;
    // A UNORM swapchain: ImGui's colours are meant as they are, with no sRGB step.
    const std::array<VkFormat, 4> formats = {VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM,
                                             VK_FORMAT_R8G8B8_UNORM};
    wd.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(vk_->physical, wd.Surface, formats.data(),
                                                             static_cast<int>(formats.size()),
                                                             VK_COLORSPACE_SRGB_NONLINEAR_KHR);
    const VkPresentModeKHR fifo = VK_PRESENT_MODE_FIFO_KHR;
    wd.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(vk_->physical, wd.Surface, &fifo, 1);
    vk_->min_image_count = static_cast<std::uint32_t>(ImGui_ImplVulkanH_GetMinImageCountFromPresentMode(wd.PresentMode));
    VkSurfaceCapabilitiesKHR caps{};
    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_->physical, wd.Surface, &caps) == VK_SUCCESS &&
        (caps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) != 0)
        vk_->extra_usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    int width = 0;
    int height = 0;
    SDL_GetWindowSizeInPixels(window_, &width, &height);
    ImGui_ImplVulkanH_CreateOrResizeWindow(vk_->instance, vk_->physical, vk_->device, &wd, vk_->queue_family, nullptr,
                                           width, height, vk_->min_image_count, vk_->extra_usage);
    return true;
}

bool Window::look_dark() const { return forced_dark_ >= 0 ? forced_dark_ == 1 : system_dark(); }

void Window::rebuild_swapchain() {
    int width = 0;
    int height = 0;
    SDL_GetWindowSizeInPixels(window_, &width, &height);
    if (width <= 0 || height <= 0) return;
    ImGui_ImplVulkan_SetMinImageCount(vk_->min_image_count);
    ImGui_ImplVulkanH_CreateOrResizeWindow(vk_->instance, vk_->physical, vk_->device, &vk_->wd, vk_->queue_family,
                                           nullptr, width, height, vk_->min_image_count, vk_->extra_usage);
    vk_->wd.FrameIndex = 0;
}

void Window::render(bool &swapchain_stale, bool capture) {
    ImDrawData *draw = ImGui::GetDrawData();
    ImGui_ImplVulkanH_Window &wd = vk_->wd;
    const ImVec4 background = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
    wd.ClearValue.color.float32[0] = background.x;
    wd.ClearValue.color.float32[1] = background.y;
    wd.ClearValue.color.float32[2] = background.z;
    wd.ClearValue.color.float32[3] = 1.0f;

    VkSemaphore acquired = wd.FrameSemaphores[wd.SemaphoreIndex].ImageAcquiredSemaphore;
    VkSemaphore complete = wd.FrameSemaphores[wd.SemaphoreIndex].RenderCompleteSemaphore;
    VkResult result = vkAcquireNextImageKHR(vk_->device, wd.Swapchain, UINT64_MAX, acquired, VK_NULL_HANDLE, &wd.FrameIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) swapchain_stale = true;
    if (result == VK_ERROR_OUT_OF_DATE_KHR) return;
    if (result != VK_SUBOPTIMAL_KHR) check(result);

    ImGui_ImplVulkanH_Frame &frame = wd.Frames[wd.FrameIndex];
    check(vkWaitForFences(vk_->device, 1, &frame.Fence, VK_TRUE, UINT64_MAX));
    check(vkResetFences(vk_->device, 1, &frame.Fence));
    check(vkResetCommandPool(vk_->device, frame.CommandPool, 0));
    VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    check(vkBeginCommandBuffer(frame.CommandBuffer, &begin));
    VkRenderPassBeginInfo pass{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
    pass.renderPass = wd.RenderPass;
    pass.framebuffer = frame.Framebuffer;
    pass.renderArea.extent.width = static_cast<std::uint32_t>(wd.Width);
    pass.renderArea.extent.height = static_cast<std::uint32_t>(wd.Height);
    pass.clearValueCount = 1;
    pass.pClearValues = &wd.ClearValue;
    vkCmdBeginRenderPass(frame.CommandBuffer, &pass, VK_SUBPASS_CONTENTS_INLINE);
    ImGui_ImplVulkan_RenderDrawData(draw, frame.CommandBuffer);
    vkCmdEndRenderPass(frame.CommandBuffer);
    capture = capture && vk_->extra_usage != 0;
    if (capture) {
        // The capture switch: the frame copied into a buffer the CPU reads.
        vk_->capture_width = wd.Width;
        vk_->capture_height = wd.Height;
        VkBufferCreateInfo buffer{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        buffer.size = static_cast<VkDeviceSize>(wd.Width) * static_cast<VkDeviceSize>(wd.Height) * 4u;
        buffer.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        check(vkCreateBuffer(vk_->device, &buffer, nullptr, &vk_->capture_buffer));
        VkMemoryRequirements needs{};
        vkGetBufferMemoryRequirements(vk_->device, vk_->capture_buffer, &needs);
        VkPhysicalDeviceMemoryProperties memory{};
        vkGetPhysicalDeviceMemoryProperties(vk_->physical, &memory);
        const VkMemoryPropertyFlags wanted = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        VkMemoryAllocateInfo allocate{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
        allocate.allocationSize = needs.size;
        for (std::uint32_t i = 0; i < memory.memoryTypeCount; ++i)
            if ((needs.memoryTypeBits & (1u << i)) != 0 && (memory.memoryTypes[i].propertyFlags & wanted) == wanted) {
                allocate.memoryTypeIndex = i;
                break;
            }
        check(vkAllocateMemory(vk_->device, &allocate, nullptr, &vk_->capture_memory));
        check(vkBindBufferMemory(vk_->device, vk_->capture_buffer, vk_->capture_memory, 0));
        VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcQueueFamilyIndex = barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = frame.Backbuffer;
        barrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
        vkCmdPipelineBarrier(frame.CommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
        VkBufferImageCopy region{};
        region.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
        region.imageExtent = {static_cast<std::uint32_t>(wd.Width), static_cast<std::uint32_t>(wd.Height), 1};
        vkCmdCopyImageToBuffer(frame.CommandBuffer, frame.Backbuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                               vk_->capture_buffer, 1, &region);
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = 0;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        vkCmdPipelineBarrier(frame.CommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0,
                             0, nullptr, 0, nullptr, 1, &barrier);
    }
    check(vkEndCommandBuffer(frame.CommandBuffer));

    const VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submit{VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submit.waitSemaphoreCount = 1;
    submit.pWaitSemaphores = &acquired;
    submit.pWaitDstStageMask = &wait_stage;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &frame.CommandBuffer;
    submit.signalSemaphoreCount = 1;
    submit.pSignalSemaphores = &complete;
    check(vkQueueSubmit(vk_->queue, 1, &submit, frame.Fence));
    if (capture) {
        check(vkWaitForFences(vk_->device, 1, &frame.Fence, VK_TRUE, UINT64_MAX));
        save_capture();
    }

    VkPresentInfoKHR present{VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    present.waitSemaphoreCount = 1;
    present.pWaitSemaphores = &complete;
    present.swapchainCount = 1;
    present.pSwapchains = &wd.Swapchain;
    present.pImageIndices = &wd.FrameIndex;
    result = vkQueuePresentKHR(vk_->queue, &present);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) swapchain_stale = true;
    else check(result);
    wd.SemaphoreIndex = (wd.SemaphoreIndex + 1) % wd.SemaphoreCount;
}

void Window::save_capture() {
    const int width = vk_->capture_width;
    const int height = vk_->capture_height;
    void *mapped = nullptr;
    if (vkMapMemory(vk_->device, vk_->capture_memory, 0, VK_WHOLE_SIZE, 0, &mapped) == VK_SUCCESS) {
        std::vector<std::uint8_t> pixels(static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4u);
        std::memcpy(pixels.data(), mapped, pixels.size());
        vkUnmapMemory(vk_->device, vk_->capture_memory);
        const VkFormat format = vk_->wd.SurfaceFormat.format;
        const bool bgra = format == VK_FORMAT_B8G8R8A8_UNORM || format == VK_FORMAT_B8G8R8A8_SRGB;
        for (std::size_t i = 0; i < pixels.size(); i += 4u) {
            if (bgra) std::swap(pixels[i], pixels[i + 2u]);
            pixels[i + 3u] = 255u;
        }
        const std::u8string name = capture_file_.u8string();
        const std::string path(name.begin(), name.end());
        if (stbi_write_png(path.c_str(), width, height, 4, pixels.data(), width * 4) != 0)
            std::cout << "[bgm-maker] captured " << width << "x" << height << " to " << path << std::endl;
        else
            std::cerr << "[bgm-maker] cannot write " << path << "\n";
    }
    vkDestroyBuffer(vk_->device, vk_->capture_buffer, nullptr);
    vkFreeMemory(vk_->device, vk_->capture_memory, nullptr);
    vk_->capture_buffer = VK_NULL_HANDLE;
    vk_->capture_memory = VK_NULL_HANDLE;
}

void Window::apply_look() { theme::apply(dark_, scale_); }

void Window::run(const std::function<bool()> &frame) {
    bool swapchain_stale = false;
    int settle = kSettleFrames;
    for (;;) {
        SDL_Event event;
        // Idle: wait for input (or half a second, for anything time-based on
        // screen). Busy: take what is there and draw at the display's rate.
        const bool scripted = script_ != nullptr && script_->active();
        bool got = settle > 0 || animating_ || capture_frames_ >= 0 || scripted ? SDL_PollEvent(&event)
                                                                                : SDL_WaitEventTimeout(&event, 500);
        while (got) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            switch (event.type) {
            case SDL_EVENT_QUIT: close_requested_ = true; break;
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                if (event.window.windowID == SDL_GetWindowID(window_)) close_requested_ = true;
                break;
            case SDL_EVENT_DROP_FILE:
                if (event.drop.data != nullptr) {
                    const std::string_view text(event.drop.data);
                    dropped_.emplace_back(std::u8string(text.begin(), text.end()));
                }
                break;
            case SDL_EVENT_SYSTEM_THEME_CHANGED:
                dark_ = look_dark();
                apply_look();
                break;
            case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED: {
                const float scale = SDL_GetWindowDisplayScale(window_);
                if (scale > 0.0f && scale != scale_) {
                    scale_ = scale;
                    apply_look();
                }
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED:
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: swapchain_stale = true; break;
            default: break;
            }
            settle = kSettleFrames;
            got = SDL_PollEvent(&event);
        }
        if (SDL_GetWindowFlags(window_) & SDL_WINDOW_MINIMIZED) {
            SDL_WaitEventTimeout(nullptr, 100);
            continue;
        }
        if (swapchain_stale) {
            rebuild_swapchain();
            swapchain_stale = false;
        }
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        // A script's input comes after the back end's, so it wins this frame.
        std::optional<std::filesystem::path> script_capture;
        if (scripted) {
            script_->before_frame();
            if (script_->close_requested()) close_requested_ = true;
            script_capture = script_->take_capture();
        }
        ImGui::NewFrame();
        const bool keep_going = frame();
        ImGui::Render();
        ImDrawData *draw = ImGui::GetDrawData();
        const bool capture = capture_frames_ == 0 || script_capture.has_value();
        if (script_capture) capture_file_ = *script_capture;
        if (draw->DisplaySize.x > 0.0f && draw->DisplaySize.y > 0.0f) render(swapchain_stale, capture);
        if (!keep_going || capture_frames_ == 0 || (script_ != nullptr && script_->finished())) break;
        if (capture_frames_ > 0) --capture_frames_;
        if (settle > 0) --settle;
    }
}

std::vector<std::filesystem::path> Window::take_dropped_files() {
    std::vector<std::filesystem::path> files;
    files.swap(dropped_);
    return files;
}

void Window::set_title(const std::string &title) {
    if (window_ != nullptr) SDL_SetWindowTitle(window_, title.c_str());
}

void Window::close() {
    if (vk_ != nullptr && vk_->device != VK_NULL_HANDLE) vkDeviceWaitIdle(vk_->device);
    if (imgui_vulkan_) ImGui_ImplVulkan_Shutdown();
    if (imgui_sdl_) ImGui_ImplSDL3_Shutdown();
    if (ImGui::GetCurrentContext() != nullptr) ImGui::DestroyContext();
    imgui_vulkan_ = imgui_sdl_ = false;
    if (vk_ != nullptr) {
        if (vk_->device != VK_NULL_HANDLE) {
            ImGui_ImplVulkanH_DestroyWindow(vk_->instance, vk_->device, &vk_->wd, nullptr);
            vkDestroyDevice(vk_->device, nullptr);
        }
        // The back end leaves the surface to whoever made it.
        if (vk_->surface != VK_NULL_HANDLE) vkDestroySurfaceKHR(vk_->instance, vk_->surface, nullptr);
        if (vk_->instance != VK_NULL_HANDLE) vkDestroyInstance(vk_->instance, nullptr);
        delete vk_;
        vk_ = nullptr;
    }
    if (window_ != nullptr) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    SDL_Quit();
}

} // namespace d012::bgm_maker
