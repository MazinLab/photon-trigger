open_project photon-trigger
set_top photon_detect
add_files src/photon_trigger.cpp
add_files src/photon_trigger.hpp
add_files -tb src/tb.cpp -cflags "-Wno-unknown-pragmas"
open_solution "solution1"
set_part {xczu28dr-ffvg1517-2-e}
create_clock -period 550MHz -name default
config_export -description {Detect photons in a phase stream} -display_name "Photon Detector" -format ip_catalog -library mkidgen3 -rtl verilog -vendor MazinLab -version 0.1
csim_design
csynth_design
#cosim_design -tool xsim

