// Export provisional pseudocode and all memory references for research use.
// Read-only: does not analyze, annotate or alter the program. Output goes to
// ignored analysis/ and is never evidence of matched source.
// @category Dissidia
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.symbol.Reference;
import java.io.BufferedWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

public class ExportResearch extends GhidraScript {
    @Override public void run() throws Exception {
        String[] args = getScriptArgs();
        Path output = Path.of(args[0]);
        Files.createDirectories(output);
        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        int count = 0, failed = 0;
        try (BufferedWriter code = Files.newBufferedWriter(output.resolve("pseudocode.c"), StandardCharsets.UTF_8);
             BufferedWriter refs = Files.newBufferedWriter(output.resolve("refs.tsv"), StandardCharsets.UTF_8)) {
            code.write("/* Provisional Ghidra pseudocode for research. Not matched source. */\n");
            refs.write("from\tto\ttype\tfunction\n");
            FunctionIterator iterator = currentProgram.getFunctionManager().getFunctions(true);
            while (iterator.hasNext() && !monitor.isCancelled()) {
                Function function = iterator.next();
                if (function.isExternal()) continue;
                Address entry = function.getEntryPoint();
                for (Instruction instruction : currentProgram.getListing().getInstructions(function.getBody(), true)) {
                    for (Reference reference : instruction.getReferencesFrom()) {
                        if (!reference.getToAddress().isMemoryAddress()) continue;
                        refs.write(instruction.getAddress() + "\t" + reference.getToAddress() + "\t"
                                + reference.getReferenceType().getName() + "\t" + entry + "\n");
                    }
                }
                code.write("\n/* @@ " + entry + " size " + function.getBody().getNumAddresses() + " */\n");
                DecompileResults result = decompiler.decompileFunction(function, 30, monitor);
                if (result.decompileCompleted()) {
                    code.write(result.getDecompiledFunction().getC());
                } else {
                    code.write("/* Decompilation failed */\n");
                    failed++;
                }
                count++;
            }
        } finally {
            decompiler.dispose();
        }
        println("Exported " + count + " functions (" + failed + " failed) to " + output);
    }
}
