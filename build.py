from architectds import *

nitrofs = NitroFS()
nitrofs.add_grit(['models'], "models")
nitrofs.generate_image()

arm9 = Arm9Binary(
  sourcedirs=['source', 'engine/source'],
  includedirs=['source', 'engine/source'],
  libs=['nds9', 'mm9', 'NE', 'c'],
  libdirs=['${BLOCKSDS}/libs/libnds', '${BLOCKSDS}/libs/maxmod', '${BLOCKSDSEXT}/nitro-engine']
  )

arm9.add_grit(["gfx"], "gfx")
arm9.generate_elf()

nds = NdsRom(
  nitrofsdirs=["nitrofiles"],
  binaries=[arm9, nitrofs],
  game_title="NQMT",
  game_subtitle="Not Quite My Tempo",
  game_author="Martin Nadaud",
  game_icon="icon.bmp",
)
nds.generate_nds()
nds.run_command_line_arguments()