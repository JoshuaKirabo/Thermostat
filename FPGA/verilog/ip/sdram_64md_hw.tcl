# TCL File Generated by Component Editor 21.1
# Thu Dec 02 11:48:30 EST 2021
# DO NOT MODIFY


# 
# sdram_64mb "sdram_64mb" v1.0
#  2021.12.02.11:48:30
# 
# 

# 
# request TCL package from ACDS 16.1
# 
package require -exact qsys 16.1


# 
# module sdram_64mb
# 
set_module_property DESCRIPTION ""
set_module_property NAME sdram_64mb
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME sdram_64mb
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL SDRAM_64MB
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file SDRAM_64MB.v VERILOG PATH SDRAM_64MB.v TOP_LEVEL_FILE


# 
# parameters
# 


# 
# display items
# 


# 
# connection point clk
# 
add_interface clk clock end
set_interface_property clk clockRate 0
set_interface_property clk ENABLED true
set_interface_property clk EXPORT_OF ""
set_interface_property clk PORT_NAME_MAP ""
set_interface_property clk CMSIS_SVD_VARIABLES ""
set_interface_property clk SVD_ADDRESS_GROUP ""

add_interface_port clk clk clk Input 1


# 
# connection point reset
# 
add_interface reset reset end
set_interface_property reset associatedClock clk
set_interface_property reset synchronousEdges DEASSERT
set_interface_property reset ENABLED true
set_interface_property reset EXPORT_OF ""
set_interface_property reset PORT_NAME_MAP ""
set_interface_property reset CMSIS_SVD_VARIABLES ""
set_interface_property reset SVD_ADDRESS_GROUP ""

add_interface_port reset reset_n reset_n Input 1


# 
# connection point s1
# 
add_interface s1 avalon end
set_interface_property s1 addressUnits WORDS
set_interface_property s1 associatedClock clk
set_interface_property s1 associatedReset reset
set_interface_property s1 bitsPerSymbol 8
set_interface_property s1 burstOnBurstBoundariesOnly false
set_interface_property s1 burstcountUnits WORDS
set_interface_property s1 explicitAddressSpan 0
set_interface_property s1 holdTime 0
set_interface_property s1 isMemoryDevice true
set_interface_property s1 linewrapBursts false
set_interface_property s1 maximumPendingReadTransactions 16
set_interface_property s1 maximumPendingWriteTransactions 0
set_interface_property s1 readLatency 0
set_interface_property s1 readWaitTime 1
set_interface_property s1 setupTime 0
set_interface_property s1 timingUnits Cycles
set_interface_property s1 writeWaitTime 0
set_interface_property s1 ENABLED true
set_interface_property s1 EXPORT_OF ""
set_interface_property s1 PORT_NAME_MAP ""
set_interface_property s1 CMSIS_SVD_VARIABLES ""
set_interface_property s1 SVD_ADDRESS_GROUP ""

add_interface_port s1 az_addr address Input 25
add_interface_port s1 az_be_n writebyteenable_n Input 2
add_interface_port s1 az_cs chipselect Input 1
add_interface_port s1 az_data writedata Input 16
add_interface_port s1 az_rd_n read_n Input 1
add_interface_port s1 za_data readdata Output 16
add_interface_port s1 za_valid readdatavalid Output 1
add_interface_port s1 za_waitrequest waitrequest Output 1
set_interface_assignment s1 embeddedsw.configuration.isFlash 0
set_interface_assignment s1 embeddedsw.configuration.isMemoryDevice 1
set_interface_assignment s1 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment s1 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point wire
# 
add_interface wire conduit end
set_interface_property wire associatedClock clk
set_interface_property wire associatedReset reset
set_interface_property wire ENABLED true
set_interface_property wire EXPORT_OF ""
set_interface_property wire PORT_NAME_MAP ""
set_interface_property wire CMSIS_SVD_VARIABLES ""
set_interface_property wire SVD_ADDRESS_GROUP ""

add_interface_port wire zs_ba ba Output 2
add_interface_port wire zs_addr addr Output 13
add_interface_port wire zs_cas_n cas_n Output 1
add_interface_port wire zs_cke cke Output 1
add_interface_port wire zs_cs_n cs_n Output 1
add_interface_port wire zs_dq dq Bidir 16
add_interface_port wire zs_dqm dqm Output 2
add_interface_port wire zs_ras_n ras_n Output 1
add_interface_port wire zs_we_n we_n Output 1

