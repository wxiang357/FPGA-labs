#----------------------------------------------------------------
#           HDMI2
#----------------------------------------------------------------
#HDMI CLK
#set_property IOSTANDARD LVDS [get_ports HDMI_CLK_P]
#set_property PACKAGE_PIN J14 [get_ports HDMI_CLK_P]
#HDMI D0
#set_property IOSTANDARD LVDS [get_ports HDMI_D0_P]
#set_property PACKAGE_PIN K13 [get_ports HDMI_D0_P]
#set_property IOSTANDARD LVDS [get_ports HDMI_D0_N]
#HDMI D1
#set_property IOSTANDARD LVDS [get_ports HDMI_D1_P]
#set_property PACKAGE_PIN G14 [get_ports HDMI_D1_P]
#HDMI D2
#set_property IOSTANDARD LVDS [get_ports HDMI_D2_P]
#set_property PACKAGE_PIN K15 [get_ports HDMI_D2_P]

# VGA
set_property PACKAGE_PIN Y21  [get_ports {vga_data[0]}];  # "VGA-B1"
set_property PACKAGE_PIN Y20  [get_ports {vga_data[1]}];  # "VGA-B2"
set_property PACKAGE_PIN AB20 [get_ports {vga_data[2]}];  # "VGA-B3"
set_property PACKAGE_PIN AB19 [get_ports {vga_data[3]}];  # "VGA-B4"
set_property PACKAGE_PIN AB22 [get_ports {vga_data[4]}];  # "VGA-G1"
set_property PACKAGE_PIN AA22 [get_ports {vga_data[5]}];  # "VGA-G2"
set_property PACKAGE_PIN AB21 [get_ports {vga_data[6]}];  # "VGA-G3"
set_property PACKAGE_PIN AA21 [get_ports {vga_data[7]}];  # "VGA-G4"
set_property PACKAGE_PIN V20  [get_ports {vga_data[8]}];  # "VGA-R1"
set_property PACKAGE_PIN U20  [get_ports {vga_data[9]}];  # "VGA-R2"
set_property PACKAGE_PIN V19  [get_ports {vga_data[10]}];  # "VGA-R3"
set_property PACKAGE_PIN V18  [get_ports {vga_data[11]}];  # "VGA-R4"
set_property IOSTANDARD LVCMOS33 [get_ports {vga_data[*]}]

set_property PACKAGE_PIN AA19 [get_ports {vga_hsync}];  # "VGA-HS"
set_property PACKAGE_PIN Y19  [get_ports {vga_vsync}];  # "VGA-VS"
set_property IOSTANDARD LVCMOS33 [get_ports vga_hsync]
set_property IOSTANDARD LVCMOS33 [get_ports vga_vsync]


#set_property SEVERITY {Warning} [get_drc_checks NSTD-1]
#set_property SEVERITY {Warning} [get_drc_checks UCIO-1]

set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[3]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[24]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[17]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[13]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[30]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[23]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[28]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[7]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[26]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[11]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[10]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[27]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[29]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[31]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[2]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[1]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[9]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[20]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[14]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[16]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[0]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[15]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[21]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[18]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[4]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[22]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[5]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[6]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[19]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[8]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[25]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[12]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[5]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[20]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[8]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[0]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[9]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[7]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[21]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[3]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[15]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[12]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[6]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[16]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[23]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[13]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[4]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[10]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[18]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[2]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[19]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[22]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[14]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[1]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[11]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[17]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/INPUT_STREAM_TLAST[0]}]
set_property MARK_DEBUG true [get_nets {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TLAST[0]}]

create_debug_core u_ila_0 ila
set_property ALL_PROBE_SAME_MU true [get_debug_cores u_ila_0]
set_property ALL_PROBE_SAME_MU_CNT 1 [get_debug_cores u_ila_0]
set_property C_ADV_TRIGGER false [get_debug_cores u_ila_0]
set_property C_DATA_DEPTH 4096 [get_debug_cores u_ila_0]
set_property C_EN_STRG_QUAL false [get_debug_cores u_ila_0]
set_property C_INPUT_PIPE_STAGES 0 [get_debug_cores u_ila_0]
set_property C_TRIGIN_EN false [get_debug_cores u_ila_0]
set_property C_TRIGOUT_EN false [get_debug_cores u_ila_0]
set_property port_width 1 [get_debug_ports u_ila_0/clk]
connect_debug_port u_ila_0/clk [get_nets [list system_i/processing_system7_0/inst/FCLK_CLK0]]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe0]
set_property port_width 24 [get_debug_ports u_ila_0/probe0]
connect_debug_port u_ila_0/probe0 [get_nets [list {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[0]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[1]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[2]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[3]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[4]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[5]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[6]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[7]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[8]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[9]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[10]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[11]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[12]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[13]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[14]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[15]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[16]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[17]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[18]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[19]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[20]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[21]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[22]} {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TDATA[23]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe1]
set_property port_width 1 [get_debug_ports u_ila_0/probe1]
connect_debug_port u_ila_0/probe1 [get_nets [list {system_i/hls_sobel_0/inst/INPUT_STREAM_TLAST[0]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe2]
set_property port_width 1 [get_debug_ports u_ila_0/probe2]
connect_debug_port u_ila_0/probe2 [get_nets [list {system_i/hls_sobel_0/inst/OUTPUT_STREAM_TLAST[0]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe3]
set_property port_width 24 [get_debug_ports u_ila_0/probe3]
connect_debug_port u_ila_0/probe3 [get_nets [list {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[0]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[1]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[2]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[3]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[4]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[5]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[6]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[7]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[8]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[9]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[10]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[11]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[12]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[13]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[14]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[15]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[16]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[17]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[18]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[19]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[20]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[21]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[22]} {system_i/hls_sobel_0/inst/INPUT_STREAM_TDATA[23]}]]
set_property C_CLK_INPUT_FREQ_HZ 300000000 [get_debug_cores dbg_hub]
set_property C_ENABLE_CLK_DIVIDER false [get_debug_cores dbg_hub]
set_property C_USER_SCAN_CHAIN 1 [get_debug_cores dbg_hub]
connect_debug_port dbg_hub/clk [get_nets u_ila_0_FCLK_CLK0]
