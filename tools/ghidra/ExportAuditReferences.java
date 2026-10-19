// Export references to reviewed overlay data islands and loader anchors.
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

public class ExportAuditReferences extends HeadlessScript {
    private String hex(Address address) {
        return String.format("0x%08X", address.getOffset());
    }

    private void collect(long start, long end, String label, ArrayList<Map<String, Object>> result) {
        var space = currentProgram.getAddressFactory().getDefaultAddressSpace();
        for (long value = start; value < end; value++) {
            Address address = space.getAddress(value);
            var references = currentProgram.getReferenceManager().getReferencesTo(address);
            while (references.hasNext()) {
                var reference = references.next();
                Function function = currentProgram.getFunctionManager().getFunctionContaining(reference.getFromAddress());
                Map<String, Object> item = new LinkedHashMap<>();
                item.put("anchor", label);
                item.put("from", hex(reference.getFromAddress()));
                item.put("to", hex(address));
                item.put("type", reference.getReferenceType().toString());
                if (function != null) item.put("source_function", hex(function.getEntryPoint()));
                result.add(item);
            }
        }
    }

    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        Path output = Path.of(args[0]);
        String module = args[1];
        var references = new ArrayList<Map<String, Object>>();
        collect(0x089DD4C0L, 0x089DD4D0L, "overlay_prefix", references);
        if (module.equals("extra")) {
            collect(0x089DE744L, 0x089DE7A0L, "middle_data_island", references);
            collect(0x089E2E38L, 0x089E33B8L, "tail_data_island", references);
        } else if (module.equals("menu")) {
            collect(0x089DD4D0L, 0x089DD568L, "undecoded_prefix", references);
            collect(0x08A08EF8L, 0x08A0A690L, "first_initializer_island", references);
            collect(0x08A93E1CL, 0x08A94700L, "float_initializer_island", references);
            collect(0x08B4BFD0L, 0x08B4BFE8L, "tail_initializer_constants", references);
        } else if (module.equals("main")) {
            var data = currentProgram.getListing().getDefinedData(true);
            while (data.hasNext()) {
                var item = data.next();
                Object value = item.getValue();
                if (value instanceof String) {
                    String string = (String)value;
                    String normalized = string.toUpperCase(java.util.Locale.ROOT);
                    if (normalized.contains("OVL_") && normalized.contains(".ELF")) {
                        collect(item.getAddress().getOffset(), item.getAddress().getOffset() + item.getLength(),
                                "overlay_filename", references);
                    }
                }
            }
        }
        Map<String, Object> report = new LinkedHashMap<>();
        report.put("module", module);
        report.put("executable_sha256", currentProgram.getExecutableSHA256());
        report.put("references", references);
        Files.createDirectories(output);
        Files.writeString(output.resolve("audit-references.json"), new GsonBuilder().setPrettyPrinting().create().toJson(report) + "\n");
        println("AUDIT REFERENCES: " + module + ", " + references.size() + " records");
    }
}
