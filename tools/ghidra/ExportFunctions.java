// Export provisional function metadata and pseudocode to ignored analysis/.
// @category Dissidia
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import com.google.gson.GsonBuilder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.Map;

public class ExportFunctions extends GhidraScript {
    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        Path output = Path.of(args[0]);
        Files.createDirectories(output);
        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        var functions = new ArrayList<Map<String, Object>>();
        StringBuilder code = new StringBuilder("/* Provisional Ghidra pseudocode. Not matched source. */\n");
        FunctionIterator iterator = currentProgram.getFunctionManager().getFunctions(true);
        try {
            while (iterator.hasNext() && !monitor.isCancelled()) {
                Function function = iterator.next();
                if (function.isExternal()) continue;
                Map<String, Object> info = new LinkedHashMap<>();
                info.put("name", function.getName());
                info.put("address", "0x" + function.getEntryPoint().toString());
                info.put("body_bytes", function.getBody().getNumAddresses());
                info.put("signature", function.getSignature().toString());
                functions.add(info);
                DecompileResults result = decompiler.decompileFunction(function, 30, monitor);
                if (result.decompileCompleted()) {
                    code.append("\n/* ").append(function.getEntryPoint()).append(" */\n");
                    code.append(result.getDecompiledFunction().getC());
                } else {
                    code.append("\n/* Decompilation failed at ").append(function.getEntryPoint()).append(" */\n");
                }
            }
            Map<String, Object> report = new LinkedHashMap<>();
            report.put("program", currentProgram.getName());
            report.put("language", currentProgram.getLanguageID().toString());
            report.put("function_count", functions.size());
            report.put("functions", functions);
            Files.writeString(output.resolve("functions.json"), new GsonBuilder().setPrettyPrinting().create().toJson(report) + "\n");
            Files.writeString(output.resolve("pseudocode.c"), code.toString());
            println("Exported " + functions.size() + " provisional functions to " + output);
        } finally {
            decompiler.dispose();
        }
    }
}
