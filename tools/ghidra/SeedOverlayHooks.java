// Seed only loader-declared entry points in a separate freshly imported project.
// @category Dissidia
import ghidra.app.util.headless.HeadlessScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import com.google.gson.GsonBuilder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.TreeSet;

public class SeedOverlayHooks extends HeadlessScript {
    private long word(long address) throws Exception {
        return Integer.toUnsignedLong(currentProgram.getMemory().getInt(toAddr(address)));
    }
    private void check(long address, long size, long end) {
        if ((address & 3) != 0 || address < 0x089DD4C0L || address + size > end)
            throw new IllegalStateException("Hook table/entry leaves overlay payload");
    }
    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        String module = args[1];
        long base = 0x089DD4C0L;
        long end = module.equals("menu") ? 0x08B59258L : 0x08B62F70L;
        int initializers = Short.toUnsignedInt(currentProgram.getMemory().getShort(toAddr(base + 4)));
        int cleanup = Short.toUnsignedInt(currentProgram.getMemory().getShort(toAddr(base + 6)));
        long initTable = word(base + 8), cleanupTable = word(base + 12);
        var entries = new TreeSet<Long>();
        check(initTable, initializers * 4L, end);
        for (int i = 0; i < initializers; i++) {
            long target = word(initTable + i * 4L);
            check(target, 8, end);
            entries.add(target);
        }
        if (cleanup > 0) {
            check(cleanupTable, cleanup * 4L, end);
            for (int i = 0; i < cleanup; i++) {
                long record = word(cleanupTable + i * 4L);
                check(record, 12, end);
                long target = word(record + 8);
                // Main-executable callbacks are resolved by the Python audit,
                // never imported into this overlay's address namespace.
                if (target < base || target >= end) continue;
                check(target, 8, end);
                entries.add(target);
            }
        }
        var results = new ArrayList<Map<String, Object>>();
        for (long entry : entries) {
            Address address = toAddr(entry);
            Function function = getFunctionAt(address);
            boolean existed = function != null;
            if (!existed) {
                if (getFunctionContaining(address) != null)
                    throw new IllegalStateException("Hook entry is inside a different function");
                if (!disassemble(address)) throw new IllegalStateException("Cannot disassemble hook entry");
                function = createFunction(address, null);
                if (function == null) throw new IllegalStateException("Cannot create hook function");
            }
            Map<String, Object> row = new LinkedHashMap<>();
            row.put("address", String.format("0x%08X", entry));
            row.put("already_present", existed);
            row.put("body_bytes", function.getBody().getNumAddresses());
            results.add(row);
        }
        analyzeChanges(currentProgram);
        var report = new LinkedHashMap<String, Object>();
        report.put("module", module);
        report.put("executable_sha256", currentProgram.getExecutableSHA256());
        report.put("entries", results);
        Files.writeString(Path.of(args[0], "hook-seeds.json"), new GsonBuilder().setPrettyPrinting().create().toJson(report) + "\n");
        println("Validated/seeded " + entries.size() + " local hook entries");
    }
}
