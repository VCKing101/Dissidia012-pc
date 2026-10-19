"""Bounded optimization-profile research; never grants source credit."""
import hashlib, io, json, re, sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools'))
from compile_psp import compile_object
from probe_compilers import functions_from_object
from mips_relocations import configured_addresses, resolve_function
CASES = [
 ('menu','tools/probes/menu_savedata_list_scoped.c',0x089DD760,464),
 ('menu','tools/probes/menu_savedata_latest_scoped.c',0x089DD9DC,396),
 ('menu','tools/probes/menu_execution_initialize.c',0x089DE3AC,412),
 ('menu','tools/probes/menu_resource_wait_optional_outer.cpp',0x089DDF24,228),
 ('main','tools/probes/main_drawing_index_transition.cpp',0x088D9F5C,200),
]
def main():
 out=ROOT/'analysis/compiler-profile-continuation';out.mkdir(parents=True,exist_ok=True)
 rows=json.loads((out/"results.json").read_text()) if "--remaining" in sys.argv else []
 addresses={}
 for m in ('main','menu'):
  manifest=json.loads((ROOT/f'config/{m}.matches.json').read_text());addresses.update(configured_addresses(manifest))
  for f in json.loads((ROOT/f'config/inventory/{m}.json').read_text())['functions']:addresses['func_'+f['address'][2:]]=int(f['address'],16)
 for module,source,address,size in (CASES[2:] if "--remaining" in sys.argv else CASES):
  spec=json.loads((ROOT/f'config/{module}.reference.json').read_text());ref=(ROOT/spec['path']).read_bytes()
  if hashlib.sha256(ref).hexdigest()!=spec['sha256']:raise ValueError('Reference drift')
  if module=='menu':offset=address-0x089DD4C0+0x54
  else:
   elf=ELFFile(io.BytesIO(ref));section=next(s for s in elf.iter_sections() if s['sh_flags']&4 and s['sh_addr']<=address<address+size<=s['sh_addr']+s['sh_size']);offset=section['sh_offset']+address-section['sh_addr']
  expected=ref[offset:offset+size];text=(ROOT/source).read_text();addresses.update({'D_'+a:int(a,16) for a in re.findall(r'\bD_([0-9A-F]{8})\b',text)})
  addresses.update({'func_'+a:int(a,16) for a in re.findall(r'\bfunc_([0-9A-F]{8})\b',text)})
  for optimization in [v for v in sys.argv[1:] if v != '--remaining'] or ['-O1','-O3','-O4']:
   tag=Path(source).stem+'_'+optimization[1:];obj=out/(tag+'.raw.o');flags=[optimization,'-g']
   compiler='tools/compilers/pspsnc_1.2.7503.0/pspsnc.exe'
   result=compile_object(compiler,flags,source,obj.relative_to(ROOT));obj.with_suffix('.log').write_text(result.stdout+result.stderr)
   if result.returncode:raise RuntimeError('Compilation failed: '+tag)
   symbols=functions_from_object(obj);name='func_%08X'%address;compiled=resolve_function(symbols[name],addresses)
   row={'module':module,'source':source,'source_sha256':hashlib.sha256((ROOT/source).read_bytes()).hexdigest(),'compiler_sha256':hashlib.sha256((ROOT/compiler).read_bytes()).hexdigest(),'name':name,'address':'0x%08X'%address,'expected_size':size,'compiled_size':len(compiled),'flags':flags,'match':compiled==expected,'different_words':sum(compiled[i:i+4]!=expected[i:i+4] for i in range(0,max(size,len(compiled)),4)),'raw_object_sha256':hashlib.sha256(obj.read_bytes()).hexdigest()}
   rows.append(row);print(name,flags,len(compiled),row['different_words'],'EXACT' if row['match'] else 'nonmatching',flush=True)
   (out/'results.json').write_text(json.dumps(rows,indent=2)+'\n')
if __name__=='__main__':main()
