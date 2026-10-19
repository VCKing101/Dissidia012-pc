// Export metadata/ranges only. Does not decompile or alter the program.
// @category Dissidia
import ghidra.app.util.headless.HeadlessScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressRange;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.mem.MemoryBlock;
import com.google.gson.GsonBuilder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.TreeSet;

public class ExportInventory extends HeadlessScript {
    private boolean fileMemory(Address address) {
        MemoryBlock block = currentProgram.getMemory().getBlock(address);
        return address.getAddressSpace().getName().equals("ram") && block != null && block.isInitialized();
    }

    private String hex(Address address) {
        return String.format("0x%08X", address.getOffset());
    }

    private ArrayList<Map<String, Object>> ranges(AddressSetView set) {
        var result = new ArrayList<Map<String, Object>>();
        var iterator = set.getAddressRanges(true);
        while (iterator.hasNext()) {
            AddressRange range = iterator.next();
            if (!fileMemory(range.getMinAddress())) continue;
            Map<String, Object> info = new LinkedHashMap<>();
            info.put("start", hex(range.getMinAddress()));
            info.put("end", String.format("0x%08X", range.getMaxAddress().getOffset() + 1));
            result.add(info);
        }
        return result;
    }

    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        Path output = Path.of(args[0]);
        String module = args.length > 1 ? args[1] : currentProgram.getName();
        Files.createDirectories(output);
        var functions = new ArrayList<Map<String, Object>>();
        var iterator = currentProgram.getFunctionManager().getFunctions(true);
        while (iterator.hasNext() && !monitor.isCancelled()) {
            Function function = iterator.next();
            if (function.isExternal() || !fileMemory(function.getEntryPoint())) continue;
            Map<String, Object> info = new LinkedHashMap<>();
            info.put("id", module + ":" + hex(function.getEntryPoint()));
            info.put("name", function.getName());
            info.put("address", hex(function.getEntryPoint()));
            info.put("body_bytes", function.getBody().getNumAddresses());
            info.put("ranges", ranges(function.getBody()));
            info.put("thunk", function.isThunk());
            info.put("confidence", "automatic-provisional");
            var calls = new TreeSet<String>();
            int indirectCalls = 0;
            var instructions = currentProgram.getListing().getInstructions(function.getBody(), true);
            while (instructions.hasNext()) {
                Instruction instruction = instructions.next();
                if (!instruction.getFlowType().isCall()) continue;
                boolean resolved = false;
                for (var reference : instruction.getReferencesFrom()) {
                    if (reference.getReferenceType().isCall()) {
                        calls.add(reference.getToAddress().toString());
                        resolved = true;
                    }
                }
                if (!resolved) indirectCalls++;
            }
            info.put("direct_call_targets", calls);
            info.put("unresolved_call_sites", indirectCalls);
            functions.add(info);
        }
        AddressSet instructionSet = new AddressSet();
        long instructionCount = 0;
        var instructions = currentProgram.getListing().getInstructions(true);
        while (instructions.hasNext() && !monitor.isCancelled()) {
            Instruction instruction = instructions.next();
            if (!fileMemory(instruction.getAddress())) continue;
            instructionSet.add(instruction.getAddress(), instruction.getMaxAddress());
            instructionCount++;
        }
        AddressSet dataSet = new AddressSet();
        var data = currentProgram.getListing().getDefinedData(true);
        long dataItems = 0;
        while (data.hasNext() && !monitor.isCancelled()) {
            Data item = data.next();
            if (!fileMemory(item.getAddress())) continue;
            dataSet.add(item.getAddress(), item.getMaxAddress());
            dataItems++;
        }
        if (monitor.isCancelled()) throw new IllegalStateException("Inventory export cancelled");
        Map<String, Object> report = new LinkedHashMap<>();
        report.put("schema_version", 1);
        report.put("module", module);
        report.put("program", currentProgram.getName());
        report.put("language", currentProgram.getLanguageID().toString());
        report.put("executable_sha256", currentProgram.getExecutableSHA256());
        boolean fresh = args.length > 2 && args[2].equals("fresh");
        report.put("analysis_mode", fresh ? "fresh_import" : "existing_read_only");
        report.put("analysis_project", args.length > 3 ? args[3] : null);
        report.put("analysis_timed_out", fresh ? analysisTimeoutOccurred() : null);
        report.put("audited", false);
        report.put("function_count", functions.size());
        report.put("functions", functions);
        report.put("instruction_count", instructionCount);
        report.put("instruction_ranges", ranges(instructionSet));
        report.put("defined_data_items", dataItems);
        report.put("defined_data_ranges", ranges(dataSet));
        Files.writeString(output.resolve("inventory.json"), new GsonBuilder().setPrettyPrinting().create().toJson(report) + "\n");
        println("INVENTORY: " + module + ", " + functions.size() + " functions, " + instructionCount + " decoded instructions");
    }
}
