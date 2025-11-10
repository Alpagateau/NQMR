import sys
#sys.path.append("../../")
from architectds import *
import os
from os.path import isfile, join, splitext
from pathlib import Path


nitrofs = NitroFS()
nitrofs.add_grit(['models'], "models")
nitrofs.add_grit(["fonts"], "fonts")
nitrofs.add_bmfont_fnt(["fonts"], "fonts")
nitrofs.generate_image()

arm9 = Arm9Binary(
  sourcedirs=['source', 'engine/source'],
  includedirs=['source', 'engine/source'],# '../../engine/include'],
  libs=['nds9', 'mm9', 'dsf','NE_debug', 'c'],
  libdirs=[
    '${BLOCKSDS}/libs/libnds', 
    '${BLOCKSDS}/libs/maxmod', 
    '${BLOCKSDSEXT}/nitro-engine',
    '${BLOCKSDSEXT}/libdsf'
    ]
  )
arm9.defines = ["NE_DEBUG"]

arm9.add_grit(["gfx"], "gfx")

arm9.generate_elf()

nds = NdsRom(
  nitrofsdirs=["nitrofiles"],
  binaries=[arm9],
  game_title="NQMT",
  game_subtitle="Not Quite My Tempo",
  game_author="Martin Nadaud",
  game_icon="icon2.bmp",
)

nds.generate_nds()
nds.run_command_line_arguments()
