// Seed independently checked dispatch roots only in a fresh project.
// @category Dissidia
import ghidra.app.util.headless.HeadlessScript;
import com.google.gson.JsonParser;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import java.nio.file.Files;
import java.nio.file.Path;

public class SeedReviewedEntries extends HeadlessScript {
    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        var manifest = JsonParser.parseString(Files.readString(Path.of(args[0]))).getAsJsonObject();
        var module = manifest.getAsJsonObject("modules").getAsJsonObject(args[1]);
        if (!module.get("reference_sha256").getAsString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))
            throw new IllegalStateException("Dispatch seeds belong to another reference");
        int added = 0;
        for (var row : module.getAsJsonArray("entries")) {
            long entry = Long.decode(row.getAsJsonObject().get("address").getAsString());
            Address address = toAddr(entry);
            var block = currentProgram.getMemory().getBlock(address);
            if ((entry & 3) != 0 || block == null || !block.isInitialized() || !block.isExecute())
                throw new IllegalStateException("Reviewed entry is not backed by executable memory");
            Function function = getFunctionAt(address);
            if (function != null) continue;
            if (getFunctionContaining(address) != null)
                throw new IllegalStateException("Reviewed entry is inside a different function");
            if (!disassemble(address) || createFunction(address, null) == null)
                throw new IllegalStateException("Cannot create reviewed dispatch function");
            added++;
        }
        analyzeChanges(currentProgram);
        println("DISPATCH ROOTS: " + args[1] + ", " + added + " missing entries seeded");
    }
}
