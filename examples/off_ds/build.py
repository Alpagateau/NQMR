import sys
from architectds import *
import os 
from os.path import isfile, join, splitext
from pathlib import Path

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
  #nitrofsdirs=["nitrofiles"],
  binaries=[arm9],
  game_title="Runtime Palette",
  game_subtitle="Test 1",
  game_author="Martin Nadaud",
  game_icon="icon.bmp",
)

nds.generate_nds()
nds.run_command_line_arguments()
