# ==================================================================
# BD700 FMZ by IceGuye.
# ==================================================================

setprop("/instrumentation/fmz/fmz-key-A-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-B-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-C-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-D-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-E-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-F-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-G-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-H-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-I-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-J-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-K-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-L-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-M-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-N-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-O-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-P-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-Q-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-R-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-S-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-T-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-U-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-V-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-W-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-X-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-Y-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-Z-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-DEL-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-CLR-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-1-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-2-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-3-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-4-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-5-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-6-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-7-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-8-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-9-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-0-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-DOT-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-DASH-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-SLASH-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-L1-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-L2-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-L3-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-L4-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-R1-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-R2-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-R3-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-R4-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-PREV-press", 0.0);
setprop("/instrumentation/fmz/fmz-key-NEXT-press", 0.0);
setprop("/instrumentation/fmz/waypoints-num", 0.0);

var waypoints = std.Vector.new();

var fmzCanvas = func {

    var font_mapper = func(family,weight) {
        if (family == "'DejaVu Sans Mono'" and weight == "normal") {
            return "DejaVuSansMono.ttf";
        }
    };

    var my_canvas = canvas.new({
        "name": "fmz",
        "size": [1024, 1024],
        "view": [1024, 1024],
        "mipmapping": 1
    });

    var fmz = my_canvas.createGroup();

    canvas.parsesvg(
        fmz,
        "Aircraft/bd700/Models/fmz/fmz.svg",
        {'font-mapper': font_mapper}
    );

    var numericMonth = {
        "JAN":1,
        "FEB":2,
        "MAR":3,
        "APR":4,
        "MAY":5,
        "JUN":6,
        "JUL":7,
        "AUG":8,
        "SEP":9,
        "OCT":10,
        "NOV":11,
        "DEC":12,
    };

    var createWaypoint = {
        orgn_id:"江泽民",
        id:"董建华",
        type:"Normal",
        lat:1989.0604,
        lon:-1989.0604,
        lat_nd:1989.0604,
        lon_nd:-1989.0604,
        speed:0,
        altitude:-19890604.0,
        altitudeCons:0,
        altitudeRestriction:"at",
        hdg_crs:0,
        hdg_crs_value:-1989.0604,
        distance:0,
        flytype:"Fly-by",
        bankLimit:25,
        radialtoIntercept:-1989.0604,
        DMEtoIntercept:-1989.0604,
        sp_turn:"Auto",
        hld_rad_or_inbd:"Inbd",
        hld_rad_vlue:-1989.0604,
        hld_turn:"Right",
        hld_time_dist:"Time",
        hld_td_value:0,
        procedure:"not in proc",

        new: func(orgn_id, lat=0.0, lon=0.0) {
            var p = {parents:[createWaypoint]};
            p.orgn_id = orgn_id;
            p.id = orgn_id;
            p.lat = lat;
            p.lon = lon;
            p.lat_nd = lat;
            p.lon_nd = lon;
            return p;
        },
    };
    
    var pageTitle = fmz.getElementById("pageTitle");
    var pageTitleRight = fmz.getElementById("pageTitleRight");
    var scratchPad = fmz.getElementById("scratchPad");
    var line1Left = fmz.getElementById("line1Left");
    var line2Left = fmz.getElementById("line2Left");
    var line3Left = fmz.getElementById("line3Left");
    var line4Left = fmz.getElementById("line4Left");
    var line5Left = fmz.getElementById("line5Left");
    var line6Left = fmz.getElementById("line6Left");
    var line7Left = fmz.getElementById("line7Left");
    var line1Right = fmz.getElementById("line1Right");
    var line2Right = fmz.getElementById("line2Right");
    var line3Right = fmz.getElementById("line3Right");
    var line4Right = fmz.getElementById("line4Right");
    var line5Right = fmz.getElementById("line5Right");
    var line6Right = fmz.getElementById("line6Right");
    var line7Right = fmz.getElementById("line7Right");
    var line1Mid = fmz.getElementById("line1Mid");
    var line2Mid = fmz.getElementById("line2Mid");
    var line3Mid = fmz.getElementById("line3Mid");
    var line4Mid = fmz.getElementById("line4Mid");
    var line5Mid = fmz.getElementById("line5Mid");
    var line6Mid = fmz.getElementById("line6Mid");
    var line7Mid = fmz.getElementById("line7Mid");
    var l7rRectangle = fmz.getElementById("l7rRectangle");
    var currentPage = fmz.getElementById("currentPage");
    var totalPage = fmz.getElementById("totalPage");
    var save_current_page = 0;
    var current_page_prop = 1;
    var total_page_prop = 1;
    var total_page_rte = 1;
    var total_page_miss = 1;
    var total_page_altn = 1;
    var page_prop_tmp = -9999;
    var scratch_pad_text = "";
    var scratch_pad_message = "";
    var scratch_pad_display = " ";
    var press_orgn = {
        "A": getprop("/instrumentation/fmz/fmz-key-A-press"),
        "B": getprop("/instrumentation/fmz/fmz-key-B-press"),
        "C": getprop("/instrumentation/fmz/fmz-key-C-press"),
        "D": getprop("/instrumentation/fmz/fmz-key-D-press"),
        "E": getprop("/instrumentation/fmz/fmz-key-E-press"),
        "F": getprop("/instrumentation/fmz/fmz-key-F-press"),
        "G": getprop("/instrumentation/fmz/fmz-key-G-press"),
        "H": getprop("/instrumentation/fmz/fmz-key-H-press"),
        "I": getprop("/instrumentation/fmz/fmz-key-I-press"),
        "J": getprop("/instrumentation/fmz/fmz-key-J-press"),
        "K": getprop("/instrumentation/fmz/fmz-key-K-press"),
        "L": getprop("/instrumentation/fmz/fmz-key-L-press"),
        "M": getprop("/instrumentation/fmz/fmz-key-M-press"),
        "N": getprop("/instrumentation/fmz/fmz-key-N-press"),
        "O": getprop("/instrumentation/fmz/fmz-key-O-press"),
        "P": getprop("/instrumentation/fmz/fmz-key-P-press"),
        "Q": getprop("/instrumentation/fmz/fmz-key-Q-press"),
        "R": getprop("/instrumentation/fmz/fmz-key-R-press"),
        "S": getprop("/instrumentation/fmz/fmz-key-S-press"),
        "T": getprop("/instrumentation/fmz/fmz-key-T-press"),
        "U": getprop("/instrumentation/fmz/fmz-key-U-press"),
        "V": getprop("/instrumentation/fmz/fmz-key-V-press"),
        "W": getprop("/instrumentation/fmz/fmz-key-W-press"),
        "X": getprop("/instrumentation/fmz/fmz-key-X-press"),
        "Y": getprop("/instrumentation/fmz/fmz-key-Y-press"),
        "Z": getprop("/instrumentation/fmz/fmz-key-Z-press"),
        "1": getprop("/instrumentation/fmz/fmz-key-1-press"),
        "2": getprop("/instrumentation/fmz/fmz-key-2-press"),
        "3": getprop("/instrumentation/fmz/fmz-key-3-press"),
        "4": getprop("/instrumentation/fmz/fmz-key-4-press"),
        "5": getprop("/instrumentation/fmz/fmz-key-5-press"),
        "6": getprop("/instrumentation/fmz/fmz-key-6-press"),
        "7": getprop("/instrumentation/fmz/fmz-key-7-press"),
        "8": getprop("/instrumentation/fmz/fmz-key-8-press"),
        "9": getprop("/instrumentation/fmz/fmz-key-9-press"),
        "0": getprop("/instrumentation/fmz/fmz-key-0-press"),
        "CLR": getprop("/instrumentation/fmz/fmz-key-CLR-press"),
        "DEL": getprop("/instrumentation/fmz/fmz-key-DEL-press"),
        ".": getprop("/instrumentation/fmz/fmz-key-DOT-press"),
        "/": getprop("/instrumentation/fmz/fmz-key-SLASH-press"),
        "-": getprop("/instrumentation/fmz/fmz-key-DASH-press"),
        "PREV": getprop("/instrumentation/fmz/fmz-key-PREV-press"),
        "NEXT": getprop("/instrumentation/fmz/fmz-key-NEXT-press"),
        "L1": getprop("/instrumentation/fmz/fmz-key-L1-press"),
        "L2": getprop("/instrumentation/fmz/fmz-key-L2-press"),
        "L3": getprop("/instrumentation/fmz/fmz-key-L3-press"),
        "L4": getprop("/instrumentation/fmz/fmz-key-L4-press"),
        "R1": getprop("/instrumentation/fmz/fmz-key-R1-press"),
        "R2": getprop("/instrumentation/fmz/fmz-key-R2-press"),
        "R3": getprop("/instrumentation/fmz/fmz-key-R3-press"),
        "R4": getprop("/instrumentation/fmz/fmz-key-R4-press"),
    };
    var A_press_orig = getprop("/instrumentation/fmz/fmz-key-A-press");
    var B_press_orig = getprop("/instrumentation/fmz/fmz-key-B-press");
    var C_press_orig = getprop("/instrumentation/fmz/fmz-key-C-press");
    var D_press_orig = getprop("/instrumentation/fmz/fmz-key-D-press");
    var E_press_orig = getprop("/instrumentation/fmz/fmz-key-E-press");
    var F_press_orig = getprop("/instrumentation/fmz/fmz-key-F-press");
    var G_press_orig = getprop("/instrumentation/fmz/fmz-key-G-press");
    var H_press_orig = getprop("/instrumentation/fmz/fmz-key-H-press");
    var I_press_orig = getprop("/instrumentation/fmz/fmz-key-I-press");
    var J_press_orig = getprop("/instrumentation/fmz/fmz-key-J-press");
    var K_press_orig = getprop("/instrumentation/fmz/fmz-key-K-press");
    var L_press_orig = getprop("/instrumentation/fmz/fmz-key-L-press");
    var M_press_orig = getprop("/instrumentation/fmz/fmz-key-M-press");
    var N_press_orig = getprop("/instrumentation/fmz/fmz-key-N-press");
    var O_press_orig = getprop("/instrumentation/fmz/fmz-key-O-press");
    var P_press_orig = getprop("/instrumentation/fmz/fmz-key-P-press");
    var Q_press_orig = getprop("/instrumentation/fmz/fmz-key-Q-press");
    var R_press_orig = getprop("/instrumentation/fmz/fmz-key-R-press");
    var S_press_orig = getprop("/instrumentation/fmz/fmz-key-S-press");
    var T_press_orig = getprop("/instrumentation/fmz/fmz-key-T-press");
    var U_press_orig = getprop("/instrumentation/fmz/fmz-key-U-press");
    var V_press_orig = getprop("/instrumentation/fmz/fmz-key-V-press");
    var W_press_orig = getprop("/instrumentation/fmz/fmz-key-W-press");
    var X_press_orig = getprop("/instrumentation/fmz/fmz-key-X-press");
    var Y_press_orig = getprop("/instrumentation/fmz/fmz-key-Y-press");
    var Z_press_orig = getprop("/instrumentation/fmz/fmz-key-Z-press");
    var DEL_press_orig = getprop("/instrumentation/fmz/fmz-key-DEL-press");
    var CLR_press_orig = getprop("/instrumentation/fmz/fmz-key-CLR-press");
    var num1_press_orig = getprop("/instrumentation/fmz/fmz-key-1-press");
    var num2_press_orig = getprop("/instrumentation/fmz/fmz-key-2-press");
    var num3_press_orig = getprop("/instrumentation/fmz/fmz-key-3-press");
    var num4_press_orig = getprop("/instrumentation/fmz/fmz-key-4-press");
    var num5_press_orig = getprop("/instrumentation/fmz/fmz-key-5-press");
    var num6_press_orig = getprop("/instrumentation/fmz/fmz-key-6-press");
    var num7_press_orig = getprop("/instrumentation/fmz/fmz-key-7-press");
    var num8_press_orig = getprop("/instrumentation/fmz/fmz-key-8-press");
    var num9_press_orig = getprop("/instrumentation/fmz/fmz-key-9-press");
    var num0_press_orig = getprop("/instrumentation/fmz/fmz-key-0-press");
    var DOT_press_orig = getprop("/instrumentation/fmz/fmz-key-DOT-press");
    var DASH_press_orig = getprop("/instrumentation/fmz/fmz-key-DASH-press");
    var SLASH_press_orig = getprop("/instrumentation/fmz/fmz-key-SLASH-press");
    var L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");
    var L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
    var L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");
    var L4_press_orig = getprop("/instrumentation/fmz/fmz-key-L4-press");
    var R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");
    var R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");
    var R3_press_orig = getprop("/instrumentation/fmz/fmz-key-R3-press");
    var R4_press_orig = getprop("/instrumentation/fmz/fmz-key-R4-press");
    var PREV_press_orig = getprop("/instrumentation/fmz/fmz-key-PREV-press");
    var NEXT_press_orig = getprop("/instrumentation/fmz/fmz-key-NEXT-press");
    var page_title_text = "NAV IDENT";
    var set_date = "10MAR16";
    var global_set_time = "1358Z";
    var time_orig = global_set_time;
    var time_counter = 0.0;
    var time_text = global_set_time;
    var time_1 = chr(time_orig[3]);
    var time_10 = chr(time_orig[2]);
    var time_100 = chr(time_orig[1]);
    var time_1000 = chr(time_orig[0]);
    var pos_init_loaded = "";
    var origin_airport_hash = createWaypoint.new(airportinfo().id, airportinfo().lat, airportinfo().lon);
    origin_airport_hash.altitude = airportinfo().elevation;
    waypoints.insert(0, origin_airport_hash);
    var origin_airport_id = waypoints.vector[0].id;
    var sld_rwy = nil;
    var wpt_sel_vec = [];
    var wpt_sel_isr = 65532;
    var wpt_num = waypoints.size();
    var custom_wpt_prop = [];
    var dest_airport = "";
    var altn_airport = "";
    var awy_wpt_list = [];
    var awy_wpt_list_lat = [];
    var awy_wpt_list_lon = [];
    var is_active = 0;
    var sel_sid = "I am angry.";
    var sel_sid_blk = "";
    var sel_sid_tran = "";
    var spd_rule = "";
    var perf_mode = "FULL PERF";
    var acft_tail_num = "";
    var clb_ias = 300;
    var clb_mach = 0.80;
    var crs_perf_type = "LRC";
    var crs_ias = 300;
    var crs_mach = 0.80;
    var des_ias = 300;
    var des_mach = 0.80;
    var des_path_deg = 3.0;
    var step_incm = 0;
    var fuel_rsv_mode = "NBAA";
    var fuel_rsv_lb = 0;
    var fuel_rsv_min = 0;
    var to_fuel = 400;
    var ldg_fuel = 200;
    var ttl_fuel_prd = 0.0;
    var dest_fuel_prd = 0.0;
    var altn_fuel_prd = 0.0;
    var fuel_no_rsv = 0.0;
    var req_rsv = 0.0;
    var ete_to_dest = 0.0;
    var ete_to_altn = 0.0;
    var dep_spd_lmt = 200;
    var dep_agl_lmt = 2500;
    var dep_nm_lmt = 4.0;
    var app_init_dst = 15;
    var app_1st_wpt = "YES";
    var cln_app_lmt = 200;
    var flp6_app_lmt = 180;
    var flp16_app_lmt = 160;
    var flp30_app_lmt = 140;
    var cln_ga_lmt = 200;
    var flp6_ga_lmt = 180;
    var flp16_ga_lmt = 160;
    var flp30_ga_lmt = 135;
    var cln_vso = 200 / 1.3;
    var flp6_vso = 180 / 1.3;
    var flp16_vso = 160 / 1.3;
    var flp30_vso = 140 / 1.3;
    var cln_vref = 200;
    var flp6_vref = 180;
    var flp16_vref = 160;
    var flp30_vref = 140;
    var miss_wpt_vec = [];
    var altn_wpt_vec = [];
    var trans_alt = 18000;
    var spd_alt_lim_s = 250;
    var spd_alt_lim_a = 10000;
    var init_crz_alt_mod = "OPTIMUM";
    var init_crz_alt = -9999;
    var crn_crz_alt = init_crz_alt;
    var opm_init_alt = 0;
    var ceil_alt = -9999;
    var isa_dev_c = 0;
    var crz_wind_d = 360;
    var crz_wind_s = 0;
    var crz_wind_at_alt = 40000;
    var bsc_oprt_wght = 45000;
    var fuel_qnt_snsr = 1;
    var total_fuel_lbs_inp = getprop("/consumables/fuel/total-fuel-lbs") or -9999;
    var plan_fuel_ground = total_fuel_lbs_inp;
    var cargo_weight = -9999;
    var pax_number = 99999;
    var weight_per_pax = -9999;
    var perf_page_read = [0,0,0,0,0];
    var fuel_prd_fom_dest = 0.2;
    var fuel_prd_fom_altn = 0.2;

    var key_prs = func(key, key_func){
        var press_crnt = getprop("/instrumentation/fmz/fmz-key-" ~ key ~ "-press");
        if (press_orgn[key] == 0 and press_crnt == 1){
            key_func();
        }
        press_orgn[key] = press_crnt;
    }

    var fmz_setlistener = func(fnc_name) {
        setlistener("sim/signals/fdm-initialized", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-A-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-B-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-C-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-D-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-E-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-F-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-G-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-H-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-I-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-J-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-K-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-L-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-M-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-N-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-O-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-P-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-Q-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-R-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-S-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-T-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-U-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-V-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-W-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-X-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-Y-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-Z-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-DEL-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-CLR-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-1-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-2-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-3-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-4-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-5-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-6-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-7-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-8-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-9-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-0-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-DOT-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-DASH-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-SLASH-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-L1-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-L2-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-L3-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-L4-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-R1-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-R2-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-R3-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-R4-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-NEXT-press", fnc_name);
        setlistener("/instrumentation/fmz/fmz-key-PREV-press", fnc_name);
    }

    var eachPageInit = func () {
        pageTitle.setText("");
        pageTitle.setColor(255,255,255,255);
        pageTitle.setFontSize(48,1);
        pageTitleRight.setText("");
        pageTitleRight.setColor(255,255,255,255);
        pageTitleRight.setFontSize(48,1);
        currentPage.setText("");
        currentPage.setColor(255,255,255,255);
        currentPage.setFontSize(36,1);
        totalPage.setText("");
        totalPage.setColor(255,255,255,255);
        totalPage.setFontSize(36,1);
        line1Left.setText("");
        line1Left.setColor(255,255,255,255);
        line1Left.setFontSize(36,1);
        line1Mid.setText("");
        line1Mid.setColor(255,255,255,255);
        line1Mid.setFontSize(36,1);
        line1Right.setText("");
        line1Right.setColor(255,255,255,255);
        line1Right.setFontSize(36,1);
        line2Left.setText("");
        line2Left.setColor(255,255,255,255);
        line2Left.setFontSize(48,1);
        line2Mid.setText("");
        line2Mid.setColor(255,255,255,255);
        line2Mid.setFontSize(36,1);
        line2Right.setText("");
        line2Right.setColor(255,255,255,255);
        line2Right.setFontSize(48,1);
        line3Left.setText("");
        line3Left.setColor(255,255,255,255);
        line3Left.setFontSize(36,1);
        line3Mid.setText("");
        line3Mid.setColor(255,255,255,255);
        line3Mid.setFontSize(36,1);
        line3Right.setText("");
        line3Right.setColor(255,255,255,255);
        line3Right.setFontSize(36,1);
        line4Left.setText("");
        line4Left.setColor(255,255,255,255);
        line4Left.setFontSize(48,1);
        line4Mid.setText("");
        line4Mid.setColor(255,255,255,255);
        line4Mid.setFontSize(36,1);
        line4Right.setText("");
        line4Right.setColor(255,255,255,255);
        line4Right.setFontSize(48,1);
        line5Left.setText("");
        line5Left.setColor(255,255,255,255);
        line5Left.setFontSize(36,1);
        line5Mid.setText("");
        line5Mid.setColor(255,255,255,255);
        line5Mid.setFontSize(36,1);
        line5Right.setText("");
        line5Right.setColor(255,255,255,255);
        line5Right.setFontSize(36,1);
        line6Left.setText("");
        line6Left.setColor(255,255,255,255);
        line6Left.setFontSize(48,1);
        line6Mid.setText("");
        line6Mid.setColor(255,255,255,255);
        line6Mid.setFontSize(36,1);
        line6Right.setText("");
        line6Right.setColor(255,255,255,255);
        line6Right.setFontSize(48,1);
        line7Left.setText("");
        line7Left.setColor(255,255,255,255);
        line7Left.setFontSize(36,1);
        line7Mid.setText("");
        line7Mid.setColor(255,255,255,255);
        line7Right.setText("");
        line7Right.setColor(255,255,255,255);
        line7Right.setFontSize(36,1);
        l7rRectangle.setVisible(0);
    }

    var bsc_oprt_wght1 = bsc_oprt_wght;
    var cargo_weight1 = cargo_weight;
    var pax_number1 = pax_number;
    var weight_per_pax1 = weight_per_pax;
    var init_crz_alt_mod1 = init_crz_alt_mod;
    var init_crz_alt1 = init_crz_alt;
    var zfw = bsc_oprt_wght1 + (weight_per_pax1 * pax_number1) + cargo_weight1;
    
    var checkPerfInit = func () {
        var can_confirm = "incompleted";
        var check_mark = [];
        var whl_cnt = 0;
        while (whl_cnt <= 5) {
            append(check_mark, 0);
            whl_cnt = whl_cnt + 1;
        }
        
        var have_all_read = 0;        
        var whl_cnt = 0;
        while (whl_cnt<size(perf_page_read)) {
            var each_have_read = perf_page_read[whl_cnt];
            if (each_have_read == 1) {
                have_all_read = 1;
                whl_cnt = whl_cnt + 1;
            } else {
                have_all_read = 0;
                whl_cnt = 65532;
            }
        }
        
        if (have_all_read == 1) {
            check_mark[0] = 1;
        }
        
        if (bsc_oprt_wght > 0) {
            check_mark[1] = 1;
        }

        if (cargo_weight >= 0) {
            check_mark[2] = 1;
        }

        if (pax_number >= 0 and pax_number <= 30) {
            check_mark[3] = 1;
        }

        if (weight_per_pax >= 0) {
            check_mark[4] = 1;
        }

        if (init_crz_alt_mod != "MANUAL" or init_crz_alt > 0) {
            check_mark[5] = 1;
        }

        if (bsc_oprt_wght1 != bsc_oprt_wght or cargo_weight1 != cargo_weight or pax_number1 != pax_number or weight_per_pax1 != weight_per_pax or init_crz_alt_mod1 != init_crz_alt_mod or init_crz_alt1 != init_crz_alt) {
            var any_perf_changed = 1;
        } else {
            var any_perf_changed = 0;
        }

        
        var whl_cnt = 0;
        while (whl_cnt < size(check_mark)) {
            if (check_mark[whl_cnt] == 1 and any_perf_changed == 1) {
                can_confirm = "can confirm";
                whl_cnt = whl_cnt + 1;
            } else if (check_mark[whl_cnt] == 1 and any_perf_changed == 0) {
                can_confirm = "don't need";
                whl_cnt = whl_cnt + 1;
            } else {
                can_confirm = "incompleted";
                whl_cnt = 65532;
            }
        }
        
        return (can_confirm);
    }

    var totalFuelByFlow = func () {
        var engine0_flow_pph = getprop("/engines/engine[0]/fuel-flow_pph") or 0;
        var engine1_flow_pph = getprop("/engines/engine[1]/fuel-flow_pph") or 0;
        var total_flow_pph = engine0_flow_pph + engine1_flow_pph;
        var total_flow_pps = total_flow_pph / 3600;
        total_fuel_lbs_inp = total_fuel_lbs_inp - total_flow_pps;
    }
    var totalFuelByFlow_timer = maketimer(1, totalFuelByFlow);
    totalFuelByFlow_timer.start();

    var frozen_fuel_on_ground = func () {
        var on_ground = getprop("/fdm/jsbsim/gear/wow") or 1;
        if (on_ground == 1) {
            plan_fuel_ground = total_fuel_lbs_inp;
        }
    }
    var frozen_fuel_on_ground_timer = maketimer(1, frozen_fuel_on_ground);
    frozen_fuel_on_ground_timer.start();

    var milthrust_grid = [
        [1.2600, 1.0000, 0.7400, 0.5340, 0.3720, 0.2410, 0.1490, 0.0000],
        [1.0680, 0.8518, 0.6357, 0.4615, 0.3238, 0.2107, 0.1304, 0.0000],
        [0.9476, 0.7589, 0.5702, 0.4169, 0.2942, 0.1920, 0.1195, 0.0000],
        [0.8692, 0.6999, 0.5307, 0.3916, 0.2782, 0.1825, 0.1133, 0.0000],
        [0.8152, 0.6610, 0.5067, 0.3771, 0.2702, 0.1782, 0.1102, 0.0000],
        [0.7666, 0.6272, 0.4878, 0.3646, 0.2660, 0.1764, 0.1092, 0.0000],
    ];

    var milthrust_inp = func (mach, alt) {
        var x = 0;
        var y = 0;
        var dif_x = 0.2;
        var dif_mach = 0;
        var dif_y = 10000;
        var dif_alt = 0;
        var thrust_cfn = 1;

        if (mach < 0.2) {
            x = 0;
            dif_mach = mach;
        } else if (mach >= 0.2 and mach < 0.4) {
            x = 1;
            dif_mach = mach - 0.2;
        } else if (mach >= 0.4 and mach < 0.6) {
            x = 2;
            dif_mach = mach - 0.4;
        } else if (mach >= 0.6 and mach < 0.8) {
            x = 3;
            dif_mach = mach - 0.6;
        } else if (mach >= 0.8 and mach < 1.0) {
            x = 4; 
            dif_mach = mach - 0.8;
        } else {
            x = 4;
            dif_mach = 0.0;
        }

        if (alt < 0) {
            y = 0;
            dif_alt = alt;
        } else if (alt >= 0 and alt < 10000) {
            y = 1;
            dif_alt = alt;
        } else if (alt >= 10000 and alt < 20000) {
            y = 2;
            dif_alt = alt - 10000;
        } else if (alt >= 20000 and alt < 30000) {
            y = 3;
            dif_alt = alt - 20000;
        } else if (alt >= 30000 and alt < 40000) {
            y = 4;
            dif_alt = alt - 30000;
        } else if (alt >= 40000 and alt < 50000) {
            y = 5;
            dif_alt = alt - 40000;
        } else if (alt >= 50000 and alt < 60000) {
            y = 6;
            dif_alt = alt - 50000;
        } else {
            y = 6;
            dif_alt = 0;
        }

        var dif_mach_y = milthrust_grid[x + 1][y] - milthrust_grid[x][y];
        var dif_mach_y1 = milthrust_grid[x + 1][y + 1] - milthrust_grid[x][y + 1];
        var avg = milthrust_grid[x][y] + dif_mach_y / dif_x * dif_mach;
        var avg1 = milthrust_grid[x][y + 1] + dif_mach_y1 / dif_x * dif_mach;
        var thrust_rsl = avg + (avg1 - avg) / dif_y * dif_alt;
        return(thrust_rsl);
    }

    var dist_to_dest = func() {
        var wpt_num = rte_wpt_counter();
        var clc_rlt = 0;
        var whl_cnt = 1;
        while (whl_cnt < wpt_num) {
            var each_dst = grtCircDist(whl_cnt);
            clc_rlt = clc_rlt + each_dst;
            whl_cnt = whl_cnt + 1;
        }
        return(clc_rlt);
    }

    var ttl_rte_dst = func() {
        var clc_rlt = 0;
        var wpt_num = waypoints.size();
        for (i=1; i<wpt_num; i=i+1){
            var each_dst = grtCircDist(i);
            clc_rlt = clc_rlt + each_dst;
        }

        return(clc_rlt);
    }

    var fuel_opm_alt_dest = func() {
    
        var grs_wgt_to = 98000;

        for (j=0; j<32; j=j+1) {
            var test_alt = 11000;
            var rte_rng = dist_to_dest();
            var rte_gone = 0;
            var ete_gone = 0;
            var grd_dns_air = getprop("/environment/density-slugft3");
            var dns_air = getprop("/environment/density-slugft3");
            var crn_dns_alt = getprop("/fdm/jsbsim/atmosphere/density-altitude");
            var crn_temp = getprop("/environment/temperature-degc");
            var fd_temp = crn_temp - ((test_alt - crn_dns_alt) / 1000 * 1.98);
            var std_temp = 15 - 1.98 * (test_alt/1000);
            var theta =  (273 + fd_temp) / (273 + 15);
            var snd_spd_temp = 661 * math.sqrt(theta);
            var oswald_efc = 0.755877483;
            var ind_drag_K = 1 / (oswald_efc * math.pi * 8.654260529);
            var cd0 = 0.013;
            var mach_conv = 0;
            var vrt_spd_toc = 99999;
            var pilot_l_wgt = getprop("/fdm/jsbsim/inertia/pointmass-weight-lbs[0]");
            var pilot_r_wgt = getprop("/fdm/jsbsim/inertia/pointmass-weight-lbs[1]");
            var cabin_pyld = getprop("/fdm/jsbsim/inertia/pointmass-weight-lbs[2]");
            zfw = bsc_oprt_wght1 + (weight_per_pax1 * pax_number1) + cargo_weight1;
            var grs_wgt_10K = grs_wgt_to;
            var grs_wgt_toc = grs_wgt_to;

            while (test_alt <= 51000 and vrt_spd_toc > 500) {

                for (i=0; i<32; i=i+1){
                    theta =  (273 + crn_temp) / (273 + 15);
                    snd_spd_temp = 661 * math.sqrt(theta);
                    var clb_tas = 250 + 250 * (crn_dns_alt/600/100 + (crn_temp - 15)/5/100);
                    mach_conv = clb_tas / snd_spd_temp;
                    var thrust = 14750 * 2 * milthrust_inp(mach_conv, crn_dns_alt);
                    var cl_for_clb = (-(thrust/grs_wgt_to) + math.sqrt(math.pow(thrust/grs_wgt_to,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    if (cl_for_clb < 0 ) {
                        cl_for_clb = (-(thrust/grs_wgt_to) - math.sqrt(math.pow(thrust/grs_wgt_to,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    }
                    var cd_for_clb = cd0 + ind_drag_K * math.pow(cl_for_clb, 2);
                    var sin_ang_clb = thrust/grs_wgt_to - cd_for_clb/cl_for_clb;
                    var vrt_spd_grd =  clb_tas * sin_ang_clb * 101.2685;

                    fd_temp = crn_temp - ((10000 - crn_dns_alt) / 1000 * 1.98);
                    std_temp = 15 - 1.98 * (10000/1000);
                    theta =  (273 + fd_temp) / (273 + 15);
                    snd_spd_temp = 661 * math.sqrt(theta);
                    var clb_tas = 250 + 250 * (10000/600/100 + (fd_temp - std_temp)/5/100);
                    mach_conv = clb_tas / snd_spd_temp;
                    if (mach_conv > clb_mach) {
                        mach_conv = clb_mach;
                    }
                    thrust = 14750 * 2 * milthrust_inp(mach_conv, 10000);
                    cl_for_clb = (-(thrust/grs_wgt_10K) + math.sqrt(math.pow(thrust/grs_wgt_10K,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    if (cl_for_clb < 0 ) {
                        cl_for_clb = (-(thrust/grs_wgt_10K) - math.sqrt(math.pow(thrust/grs_wgt_10K,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    }
                    cd_for_clb = cd0 + ind_drag_K * math.pow(cl_for_clb, 2);
                    sin_ang_clb = thrust/grs_wgt_10K - cd_for_clb/cl_for_clb;
                    var vrt_spd_10K =  clb_tas * sin_ang_clb * 101.2685;
                    var abs_alt_lmt = (crn_dns_alt * vrt_spd_10K - 10000 * vrt_spd_grd) / (vrt_spd_10K - vrt_spd_grd);
                    var clb_time_10K = (abs_alt_lmt / vrt_spd_grd) * math.ln((abs_alt_lmt - crn_dns_alt) / (abs_alt_lmt - 10000));
                    clb_time_10K = clb_time_10K / 60;
                    var fuel_burned_10K = clb_time_10K * thrust * 0.623;
                    grs_wgt_10K = grs_wgt_to - fuel_burned_10K;
                }
                rte_gone = clb_tas * clb_time_10K;
                ete_gone = clb_time_10K;

                for (i=0; i<32; i=i+1) {
                    fd_temp = crn_temp - ((test_alt - crn_dns_alt) / 1000 * 1.98);
                    std_temp = 15 - 1.98 * (test_alt/1000);
                    theta =  (273 + fd_temp) / (273 + 15);
                    snd_spd_temp = 661 * math.sqrt(theta);
                    var clb_tas = clb_ias + clb_ias * (test_alt/600/100 + (fd_temp - std_temp)/5/100);
                    mach_conv = clb_tas / snd_spd_temp;
                    if (mach_conv > clb_mach) {
                        mach_conv = clb_mach;
                    }
                    thrust = 14750 * 2 * milthrust_inp(mach_conv, test_alt);
                    cl_for_clb = (-(thrust/grs_wgt_toc) + math.sqrt(math.pow(thrust/grs_wgt_toc,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    if (cl_for_clb < 0 ) {
                        cl_for_clb = (-(thrust/grs_wgt_toc) - math.sqrt(math.pow(thrust/grs_wgt_toc,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    }
                    cd_for_clb = cd0 + ind_drag_K * math.pow(cl_for_clb, 2);
                    sin_ang_clb = thrust/grs_wgt_toc - cd_for_clb/cl_for_clb;
                    if (mach_conv < clb_mach) {
                        vrt_spd_toc =  clb_tas * sin_ang_clb * 101.2685;
                    } else {
                        var clb_tas = mach_conv * snd_spd_temp;
                        vrt_spd_toc = clb_tas * sin_ang_clb * 101.2685;
                    }
                    var clb_time_toc = 0;
                    if ((abs_alt_lmt - 10000) / (abs_alt_lmt - test_alt) != 1) {
                        abs_alt_lmt = (10000 * vrt_spd_toc - test_alt * vrt_spd_10K) / (vrt_spd_toc - vrt_spd_10K);
                        clb_time_toc = (abs_alt_lmt / vrt_spd_10K) * math.ln((abs_alt_lmt - 10000) / (abs_alt_lmt - test_alt));
                    }
                    clb_time_toc = clb_time_toc / 60;
                    var fuel_burned_toc = clb_time_toc * thrust * 0.623;
                    grs_wgt_toc = grs_wgt_10K - fuel_burned_toc;
                }
                test_alt = test_alt + 1000;
            }
            opm_init_alt = test_alt - 1000 - 2000;
            ceil_alt = test_alt - 1000;
            var opm_alt = test_alt - 1000 - 2000;

            for (i=0; i<32; i=i+1) {
                fd_temp = crn_temp - ((opm_alt - crn_dns_alt) / 1000 * 1.98);
                std_temp = 15 - 1.98 * (opm_alt/1000);
                theta =  (273 + fd_temp) / (273 + 15);
                snd_spd_temp = 661 * math.sqrt(theta);
                var clb_tas = clb_ias + clb_ias * (opm_alt/600/100 + (fd_temp - std_temp)/5/100);
                mach_conv = clb_tas / snd_spd_temp;
                if (mach_conv > clb_mach) {
                    mach_conv = clb_mach;
                }
                thrust = 14750 * 2 * milthrust_inp(mach_conv, opm_alt);
                cl_for_clb = (-(thrust/grs_wgt_toc) + math.sqrt(math.pow(thrust/grs_wgt_toc,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                if (cl_for_clb < 0 ) {
                    cl_for_clb = (-(thrust/grs_wgt_toc) - math.sqrt(math.pow(thrust/grs_wgt_toc,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                }
                cd_for_clb = cd0 + ind_drag_K * math.pow(cl_for_clb, 2);
                sin_ang_clb = thrust/grs_wgt_toc - cd_for_clb/cl_for_clb;
                if (mach_conv < clb_mach) {
                    vrt_spd_toc =  clb_tas * sin_ang_clb * 101.2685;
                } else {
                    var clb_tas = mach_conv * snd_spd_temp;
                    vrt_spd_toc = clb_tas * sin_ang_clb * 101.2685;
                }
                var clb_time_toc = 0;
                if ((abs_alt_lmt - 10000) / (abs_alt_lmt - opm_alt) != 1) {
                    abs_alt_lmt = (10000 * vrt_spd_toc - opm_alt * vrt_spd_10K) / (vrt_spd_toc - vrt_spd_10K);
                    clb_time_toc = (abs_alt_lmt / vrt_spd_10K) * math.ln((abs_alt_lmt - 10000) / (abs_alt_lmt - opm_alt));
                }
                clb_time_toc = clb_time_toc / 60;
                var fuel_burned_toc = clb_time_toc * thrust * 0.623;
                grs_wgt_toc = grs_wgt_10K - fuel_burned_toc;
            }
            rte_gone = rte_gone + (clb_time_toc * clb_tas);
            ete_gone = ete_gone + clb_time_toc;

            var opm_fd_temp = crn_temp - ((opm_alt - crn_dns_alt) / 1000 * 1.98);
            var opm_std_temp = 15 - 1.98 * (opm_alt/1000);
            var opm_alt_prs = opm_alt - (100 * (opm_fd_temp - opm_std_temp));
            opm_alt_prs = math.round(opm_alt / 1000) * 1000;

            var grs_wgt_crs = grs_wgt_toc;

            while(rte_gone < rte_rng) {
                vrt_spd_toc = -99999;
                var crs_min_count = 0;
                while(vrt_spd_toc < 500 and rte_gone < rte_rng) {

                    var opm_alt_std_temp = 15 - 1.98 * (opm_alt / 1000);
                    theta =  (273 + opm_alt_std_temp) / (273 + 15);
                    snd_spd_temp = 661 * math.sqrt(theta);
                    var crs_tas = crs_ias * (1 + (opm_alt/600/100));
                    mach_conv = crs_tas / snd_spd_temp;

                    if (mach_conv > crs_mach) {
                        mach_conv = crs_mach;
                    }

                    var crs_thr = 14750 * 2 * milthrust_inp(mach_conv, opm_alt);
                    var sin_ang_crs = 99999;
                    while(sin_ang_crs>0.008){
                        crs_thr = crs_thr - 100;
                        cl_for_crs = (-(crs_thr/grs_wgt_crs) + math.sqrt(math.pow(crs_thr/grs_wgt_crs,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                        if (cl_for_crs < 0 ) {
                            cl_for_crs = (-(crs_thr/grs_wgt_crs) - math.sqrt(math.pow(crs_thr/grs_wgt_crs,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                        }
                        cd_for_crs = cd0 + ind_drag_K * math.pow(cl_for_crs, 2);
                        sin_ang_crs = crs_thr/grs_wgt_crs - cd_for_crs/cl_for_crs;
                    }
                    var crs_fuel_rate = (crs_thr * 0.623);
                    var crs_fuel_brn_min = crs_fuel_rate / 60;
                    grs_wgt_crs = grs_wgt_crs - crs_fuel_brn_min;

                    var next_opm_alt = opm_alt + (step_incm * 1000) + 2000;

                    var next_std_temp = 15 - 1.98 * (next_opm_alt / 1000);
                    theta =  (273 + next_std_temp) / (273 + 15);
                    snd_spd_temp = 661 * math.sqrt(theta);
                    var clb_tas = clb_ias + clb_ias * (next_opm_alt/600/100);
                    mach_conv = clb_tas / snd_spd_temp;
                    if (mach_conv > clb_mach) {
                        mach_conv = clb_mach;
                    }
                    thrust = 14750 * 2 * milthrust_inp(mach_conv, next_opm_alt);
                    cl_for_clb = (-(thrust/grs_wgt_crs) + math.sqrt(math.pow(thrust/grs_wgt_crs,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    if (cl_for_clb < 0 ) {
                        cl_for_clb = (-(thrust/grs_wgt_crs) - math.sqrt(math.pow(thrust/grs_wgt_crs,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    }
                    cd_for_clb = cd0 + ind_drag_K * math.pow(cl_for_clb, 2);
                    sin_ang_clb = thrust/grs_wgt_crs - cd_for_clb/cl_for_clb;
                    if (mach_conv < clb_mach) {
                        vrt_spd_toc =  clb_tas * sin_ang_clb * 101.2685;
                    } else {
                        var clb_tas = mach_conv * snd_spd_temp;
                        vrt_spd_toc = clb_tas * sin_ang_clb * 101.2685;
                    }
                    crs_min_count = crs_min_count + 1;
                    rte_gone = rte_gone + (1/60 * crs_tas);
                    ete_gone = ete_gone + (1/60);
                }

                if (vrt_spd_toc > 500) {
                    var last_opm_alt = opm_alt;
                    opm_alt = next_opm_alt - 2000;

                    var vrt_spd_next_step = -999999;
                    var grs_wgt_next_step = grs_wgt_crs;

                    for (i=0; i<8; i=i+1) {
                        std_temp = 15 - 1.98 * (opm_alt/1000);
                        theta =  (273 + std_temp) / (273 + 15);
                        snd_spd_temp = 661 * math.sqrt(theta);
                        var clb_tas = clb_ias + clb_ias * (opm_alt/600/100);
                        mach_conv = clb_tas / snd_spd_temp;
                        if (mach_conv > clb_mach) {
                            mach_conv = clb_mach;
                        }
                        thrust = 14750 * 2 * milthrust_inp(mach_conv, opm_alt);
                        cl_for_clb = (-(thrust/grs_wgt_next_step) + math.sqrt(math.pow(thrust/grs_wgt_next_step,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                        if (cl_for_clb < 0 ) {
                            cl_for_clb = (-(thrust/grs_wgt_next_step) - math.sqrt(math.pow(thrust/grs_wgt_next_step,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                        }
                        cd_for_clb = cd0 + ind_drag_K * math.pow(cl_for_clb, 2);
                        sin_ang_clb = thrust/grs_wgt_next_step - cd_for_clb/cl_for_clb;
                        if (mach_conv < clb_mach) {
                            vrt_spd_next_step =  clb_tas * sin_ang_clb * 101.2685;
                        } else {
                            var clb_tas = mach_conv * snd_spd_temp;
                            vrt_spd_next_step = clb_tas * sin_ang_clb * 101.2685;
                        }
                        var clb_time_toc = 0;
                        if ((abs_alt_lmt - last_opm_alt) / (abs_alt_lmt - opm_alt) != 1) {
                            abs_alt_lmt = (last_opm_alt * vrt_spd_next_step - opm_alt * vrt_spd_toc) / (vrt_spd_next_step - vrt_spd_toc);
                            clb_time_toc = (abs_alt_lmt / vrt_spd_toc) * math.ln((abs_alt_lmt - last_opm_alt) / (abs_alt_lmt - opm_alt));
                        }
                        clb_time_toc = clb_time_toc / 60;
                        var fuel_burned_toc = clb_time_toc * thrust * 0.623;
                        grs_wgt_next_step = grs_wgt_crs - fuel_burned_toc;
                    }
                    rte_gone = rte_gone + (clb_time_toc * clb_tas);
                    ete_gone = ete_gone + clb_time_toc;
                    grs_wgt_crs = grs_wgt_next_step;
                }
            }

            if (fuel_rsv_mode == "NBAA") {
                
                var grs_wgt_ga_5K = grs_wgt_crs;

                for (i=0; i<8; i=i+1){
                    theta =  (273 + std_temp) / (273 + 15);
                    snd_spd_temp = 661 * math.sqrt(theta);
                    var clb_tas = 200 + 200 * (crn_dns_alt/600/100);
                    mach_conv = clb_tas / snd_spd_temp;
                    var thrust = 14750 * 2 * milthrust_inp(mach_conv, crn_dns_alt);
                    var cl_for_clb = (-(thrust/grs_wgt_crs) + math.sqrt(math.pow(thrust/grs_wgt_crs,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    if (cl_for_clb < 0 ) {
                        cl_for_clb = (-(thrust/grs_wgt_crs) - math.sqrt(math.pow(thrust/grs_wgt_crs,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    }
                    var cd_for_clb = cd0 + ind_drag_K * math.pow(cl_for_clb, 2);
                    var sin_ang_clb = thrust/grs_wgt_crs - cd_for_clb/cl_for_clb;
                    var vrt_spd_grd =  clb_tas * sin_ang_clb * 101.2685;

                    std_temp = 15 - 1.98 * (5000/1000);
                    theta =  (273 + std_temp) / (273 + 15);
                    snd_spd_temp = 661 * math.sqrt(theta);
                    var clb_tas = 250 + 250 * (5000/600/100);
                    mach_conv = clb_tas / snd_spd_temp;
                    if (mach_conv > clb_mach) {
                        mach_conv = clb_mach;
                    }
                    thrust = 14750 * 2 * milthrust_inp(mach_conv, 5000);
                    cl_for_clb = (-(thrust/grs_wgt_ga_5K) + math.sqrt(math.pow(thrust/grs_wgt_ga_5K,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    if (cl_for_clb < 0 ) {
                        cl_for_clb = (-(thrust/grs_wgt_ga_5K) - math.sqrt(math.pow(thrust/grs_wgt_ga_5K,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    }
                    cd_for_clb = cd0 + ind_drag_K * math.pow(cl_for_clb, 2);
                    sin_ang_clb = thrust/grs_wgt_ga_5K - cd_for_clb/cl_for_clb;
                    var vrt_spd_5K =  clb_tas * sin_ang_clb * 101.2685;
                    var abs_alt_lmt = (crn_dns_alt * vrt_spd_5K - 5000 * vrt_spd_grd) / (vrt_spd_5K - vrt_spd_grd);
                    var clb_time_5K = (abs_alt_lmt / vrt_spd_grd) * math.ln((abs_alt_lmt - crn_dns_alt) / (abs_alt_lmt - 5000));
                    clb_time_5K = clb_time_5K / 60;
                    var fuel_burned_5K = clb_time_10K * thrust * 0.623;
                    grs_wgt_ga_5K = grs_wgt_crs - fuel_burned_5K;
                }

                var nbaa_atn_gone = 0;
                ete_gone = ete_gone + clb_time_5K;
                var grs_wgt_atn = grs_wgt_ga_5K;

                while(nbaa_atn_gone <= 200) {

                    var opm_alt_std_temp = 15 - 1.98 * (opm_alt / 1000);
                    theta =  (273 + opm_alt_std_temp) / (273 + 15);
                    snd_spd_temp = 661 * math.sqrt(theta);
                    var crs_tas = crs_ias * (1 + (opm_alt/600/100));
                    mach_conv = crs_tas / snd_spd_temp;

                    if (mach_conv > crs_mach) {
                        mach_conv = crs_mach;
                    }

                    var crs_thr = 14750 * 2 * milthrust_inp(mach_conv, opm_alt);
                    var sin_ang_crs = 99999;
                    while(sin_ang_crs>0.008){
                        crs_thr = crs_thr - 100;
                        cl_for_crs = (-(crs_thr/grs_wgt_atn) + math.sqrt(math.pow(crs_thr/grs_wgt_atn,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                        if (cl_for_crs < 0 ) {
                            cl_for_crs = (-(crs_thr/grs_wgt_atn) - math.sqrt(math.pow(crs_thr/grs_wgt_atn,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                        }
                        cd_for_crs = cd0 + ind_drag_K * math.pow(cl_for_crs, 2);
                        sin_ang_crs = crs_thr/grs_wgt_atn - cd_for_crs/cl_for_crs;
                    }

                    var atn_fuel_rate = (crs_thr * 0.623);
                    var atn_fuel_brn_min = atn_fuel_rate / 60;
                    grs_wgt_atn = grs_wgt_atn - atn_fuel_brn_min;
                    nbaa_atn_gone = nbaa_atn_gone + (crs_tas * (1/60));
                    ete_gone = ete_gone + (1/60);
                }

                var grs_wgt_hold = grs_wgt_atn;

                for (hold_min=0; hold_min<=35; hold_min=hold_min+1) {
                    var hold_std_temp = 15 - 1.98 * (5000 / 1000);
                    var hold_tas = 200 * (1 + (5000/600/100));
                    var hold_thr = 14750 * 2 * milthrust_inp(mach_conv, 5000);
                    var sin_ang_hold = 99999;
                    while(sin_ang_hold>0.008){
                        hold_thr = hold_thr - 100;
                        hold_cl = (-(hold_thr/grs_wgt_hold) + math.sqrt(math.pow(hold_thr/grs_wgt_hold,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                        if (hold_cl < 0 ) {
                            hold_cl = (-(hold_thr/grs_wgt_hold) - math.sqrt(math.pow(hold_thr/grs_wgt_hold,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                        }
                        hold_cd = cd0 + ind_drag_K * math.pow(hold_cl, 2);
                        sin_ang_hold = hold_thr/grs_wgt_hold - hold_cd/hold_cl;
                    }
                    var hold_fuel_rate = (hold_thr * 0.623);
                    var hold_fuel_brn_min = hold_fuel_rate / 60;
                    grs_wgt_hold = grs_wgt_hold - hold_fuel_brn_min;
                }
                ete_gone = ete_gone + (35/60);
               
                var grs_wgt_ld = grs_wgt_hold;
                ttl_fuel_prd = grs_wgt_to - grs_wgt_ld + to_fuel + ldg_fuel;
                
            } else if (fuel_rsv_lb != 0) {
                ttl_fuel_prd = grs_wgt_to - grs_wgt_crs + fuel_rsv_lb + to_fuel + ldg_fuel;
            }
            fuel_no_rsv = grs_wgt_to - grs_wgt_crs + to_fuel + ldg_fuel;
            req_rsv = ttl_fuel_prd - fuel_no_rsv;
            grs_wgt_to = zfw + ttl_fuel_prd;
            if (grs_wgt_to > 98000) {
                grs_wgt_to = 98000;
            }
        }
        dest_fuel_prd = ttl_fuel_prd;
        ete_to_dest = ete_gone;
    }

    var fuel_opm_alt_altn = func() {
        var grs_wgt_to = 98000;

        for (j=0; j<32; j=j+1) {
            var test_alt = 11000;
            var rte_rng = ttl_rte_dst();
            var rte_gone = 0;
            var ete_gone = 0;
            var grd_dns_air = getprop("/environment/density-slugft3");
            var dns_air = getprop("/environment/density-slugft3");
            var crn_dns_alt = getprop("/fdm/jsbsim/atmosphere/density-altitude");
            var crn_temp = getprop("/environment/temperature-degc");
            var fd_temp = crn_temp - ((test_alt - crn_dns_alt) / 1000 * 1.98);
            var std_temp = 15 - 1.98 * (test_alt/1000);
            var theta =  (273 + fd_temp) / (273 + 15);
            var snd_spd_temp = 661 * math.sqrt(theta);
            var oswald_efc = 0.755877483;
            var ind_drag_K = 1 / (oswald_efc * math.pi * 8.654260529);
            var cd0 = 0.013;
            var mach_conv = 0;
            var vrt_spd_toc = 99999;
            var pilot_l_wgt = getprop("/fdm/jsbsim/inertia/pointmass-weight-lbs[0]");
            var pilot_r_wgt = getprop("/fdm/jsbsim/inertia/pointmass-weight-lbs[1]");
            var cabin_pyld = getprop("/fdm/jsbsim/inertia/pointmass-weight-lbs[2]");
            zfw = bsc_oprt_wght1 + (weight_per_pax1 * pax_number1) + cargo_weight1;
            var grs_wgt_10K = grs_wgt_to;
            var grs_wgt_toc = grs_wgt_to;

            while (test_alt <= 51000 and vrt_spd_toc > 500) {

                for (i=0; i<32; i=i+1){
                    theta =  (273 + crn_temp) / (273 + 15);
                    snd_spd_temp = 661 * math.sqrt(theta);
                    var clb_tas = 250 + 250 * (crn_dns_alt/600/100 + (crn_temp - 15)/5/100);
                    mach_conv = clb_tas / snd_spd_temp;
                    var thrust = 14750 * 2 * milthrust_inp(mach_conv, crn_dns_alt);
                    var cl_for_clb = (-(thrust/grs_wgt_to) + math.sqrt(math.pow(thrust/grs_wgt_to,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    if (cl_for_clb < 0 ) {
                        cl_for_clb = (-(thrust/grs_wgt_to) - math.sqrt(math.pow(thrust/grs_wgt_to,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    }
                    var cd_for_clb = cd0 + ind_drag_K * math.pow(cl_for_clb, 2);
                    var sin_ang_clb = thrust/grs_wgt_to - cd_for_clb/cl_for_clb;
                    var vrt_spd_grd =  clb_tas * sin_ang_clb * 101.2685;

                    fd_temp = crn_temp - ((10000 - crn_dns_alt) / 1000 * 1.98);
                    std_temp = 15 - 1.98 * (10000/1000);
                    theta =  (273 + fd_temp) / (273 + 15);
                    snd_spd_temp = 661 * math.sqrt(theta);
                    var clb_tas = 250 + 250 * (10000/600/100 + (fd_temp - std_temp)/5/100);
                    mach_conv = clb_tas / snd_spd_temp;
                    if (mach_conv > clb_mach) {
                        mach_conv = clb_mach;
                    }
                    thrust = 14750 * 2 * milthrust_inp(mach_conv, 10000);
                    cl_for_clb = (-(thrust/grs_wgt_10K) + math.sqrt(math.pow(thrust/grs_wgt_10K,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    if (cl_for_clb < 0 ) {
                        cl_for_clb = (-(thrust/grs_wgt_10K) - math.sqrt(math.pow(thrust/grs_wgt_10K,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    }
                    cd_for_clb = cd0 + ind_drag_K * math.pow(cl_for_clb, 2);
                    sin_ang_clb = thrust/grs_wgt_10K - cd_for_clb/cl_for_clb;
                    var vrt_spd_10K =  clb_tas * sin_ang_clb * 101.2685;
                    var abs_alt_lmt = (crn_dns_alt * vrt_spd_10K - 10000 * vrt_spd_grd) / (vrt_spd_10K - vrt_spd_grd);
                    var clb_time_10K = (abs_alt_lmt / vrt_spd_grd) * math.ln((abs_alt_lmt - crn_dns_alt) / (abs_alt_lmt - 10000));
                    clb_time_10K = clb_time_10K / 60;
                    var fuel_burned_10K = clb_time_10K * thrust * 0.623;
                    grs_wgt_10K = grs_wgt_to - fuel_burned_10K;
                }
                rte_gone = clb_tas * clb_time_10K;
                ete_gone = clb_time_10K;

                for (i=0; i<32; i=i+1) {
                    fd_temp = crn_temp - ((test_alt - crn_dns_alt) / 1000 * 1.98);
                    std_temp = 15 - 1.98 * (test_alt/1000);
                    theta =  (273 + fd_temp) / (273 + 15);
                    snd_spd_temp = 661 * math.sqrt(theta);
                    var clb_tas = clb_ias + clb_ias * (test_alt/600/100 + (fd_temp - std_temp)/5/100);
                    mach_conv = clb_tas / snd_spd_temp;
                    if (mach_conv > clb_mach) {
                        mach_conv = clb_mach;
                    }
                    thrust = 14750 * 2 * milthrust_inp(mach_conv, test_alt);
                    cl_for_clb = (-(thrust/grs_wgt_toc) + math.sqrt(math.pow(thrust/grs_wgt_toc,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    if (cl_for_clb < 0 ) {
                        cl_for_clb = (-(thrust/grs_wgt_toc) - math.sqrt(math.pow(thrust/grs_wgt_toc,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    }
                    cd_for_clb = cd0 + ind_drag_K * math.pow(cl_for_clb, 2);
                    sin_ang_clb = thrust/grs_wgt_toc - cd_for_clb/cl_for_clb;
                    if (mach_conv < clb_mach) {
                        vrt_spd_toc =  clb_tas * sin_ang_clb * 101.2685;
                    } else {
                        var clb_tas = mach_conv * snd_spd_temp;
                        vrt_spd_toc = clb_tas * sin_ang_clb * 101.2685;
                    }
                    var clb_time_toc = 0;
                    if ((abs_alt_lmt - 10000) / (abs_alt_lmt - test_alt) != 1) {
                        abs_alt_lmt = (10000 * vrt_spd_toc - test_alt * vrt_spd_10K) / (vrt_spd_toc - vrt_spd_10K);
                        clb_time_toc = (abs_alt_lmt / vrt_spd_10K) * math.ln((abs_alt_lmt - 10000) / (abs_alt_lmt - test_alt));
                    }
                    clb_time_toc = clb_time_toc / 60;
                    var fuel_burned_toc = clb_time_toc * thrust * 0.623;
                    grs_wgt_toc = grs_wgt_10K - fuel_burned_toc;
                }
                test_alt = test_alt + 1000;
            }
            opm_init_alt = test_alt - 1000 - 2000;
            ceil_alt = test_alt - 1000;
            var opm_alt = test_alt - 1000 - 2000;

            for (i=0; i<32; i=i+1) {
                fd_temp = crn_temp - ((opm_alt - crn_dns_alt) / 1000 * 1.98);
                std_temp = 15 - 1.98 * (opm_alt/1000);
                theta =  (273 + fd_temp) / (273 + 15);
                snd_spd_temp = 661 * math.sqrt(theta);
                var clb_tas = clb_ias + clb_ias * (opm_alt/600/100 + (fd_temp - std_temp)/5/100);
                mach_conv = clb_tas / snd_spd_temp;
                if (mach_conv > clb_mach) {
                    mach_conv = clb_mach;
                }
                thrust = 14750 * 2 * milthrust_inp(mach_conv, opm_alt);
                cl_for_clb = (-(thrust/grs_wgt_toc) + math.sqrt(math.pow(thrust/grs_wgt_toc,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                if (cl_for_clb < 0 ) {
                    cl_for_clb = (-(thrust/grs_wgt_toc) - math.sqrt(math.pow(thrust/grs_wgt_toc,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                }
                cd_for_clb = cd0 + ind_drag_K * math.pow(cl_for_clb, 2);
                sin_ang_clb = thrust/grs_wgt_toc - cd_for_clb/cl_for_clb;
                if (mach_conv < clb_mach) {
                    vrt_spd_toc =  clb_tas * sin_ang_clb * 101.2685;
                } else {
                    var clb_tas = mach_conv * snd_spd_temp;
                    vrt_spd_toc = clb_tas * sin_ang_clb * 101.2685;
                }
                var clb_time_toc = 0;
                if ((abs_alt_lmt - 10000) / (abs_alt_lmt - opm_alt) != 1) {
                    abs_alt_lmt = (10000 * vrt_spd_toc - opm_alt * vrt_spd_10K) / (vrt_spd_toc - vrt_spd_10K);
                    clb_time_toc = (abs_alt_lmt / vrt_spd_10K) * math.ln((abs_alt_lmt - 10000) / (abs_alt_lmt - opm_alt));
                }
                clb_time_toc = clb_time_toc / 60;
                var fuel_burned_toc = clb_time_toc * thrust * 0.623;
                grs_wgt_toc = grs_wgt_10K - fuel_burned_toc;
            }
            rte_gone = rte_gone + (clb_time_toc * clb_tas);
            ete_gone = ete_gone + clb_time_toc;

            var opm_fd_temp = crn_temp - ((opm_alt - crn_dns_alt) / 1000 * 1.98);
            var opm_std_temp = 15 - 1.98 * (opm_alt/1000);
            var opm_alt_prs = opm_alt - (100 * (opm_fd_temp - opm_std_temp));
            opm_alt_prs = math.round(opm_alt / 1000) * 1000;

            var grs_wgt_crs = grs_wgt_toc;

            while(rte_gone < rte_rng) {
                vrt_spd_toc = -99999;
                var crs_min_count = 0;
                while(vrt_spd_toc < 500 and rte_gone < rte_rng) {

                    var opm_alt_std_temp = 15 - 1.98 * (opm_alt / 1000);
                    theta =  (273 + opm_alt_std_temp) / (273 + 15);
                    snd_spd_temp = 661 * math.sqrt(theta);
                    var crs_tas = crs_ias * (1 + (opm_alt/600/100));
                    mach_conv = crs_tas / snd_spd_temp;

                    if (mach_conv > crs_mach) {
                        mach_conv = crs_mach;
                    }

                    var crs_thr = 14750 * 2 * milthrust_inp(mach_conv, opm_alt);
                    var sin_ang_crs = 99999;
                    while(sin_ang_crs>0.008){
                        crs_thr = crs_thr - 100;
                        cl_for_crs = (-(crs_thr/grs_wgt_crs) + math.sqrt(math.pow(crs_thr/grs_wgt_crs,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                        if (cl_for_crs < 0 ) {
                            cl_for_crs = (-(crs_thr/grs_wgt_crs) - math.sqrt(math.pow(crs_thr/grs_wgt_crs,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                        }
                        cd_for_crs = cd0 + ind_drag_K * math.pow(cl_for_crs, 2);
                        sin_ang_crs = crs_thr/grs_wgt_crs - cd_for_crs/cl_for_crs;
                    }
                    var crs_fuel_rate = (crs_thr * 0.623);
                    var crs_fuel_brn_min = crs_fuel_rate / 60;
                    grs_wgt_crs = grs_wgt_crs - crs_fuel_brn_min;

                    var next_opm_alt = opm_alt + (step_incm * 1000) + 2000;

                    var next_std_temp = 15 - 1.98 * (next_opm_alt / 1000);
                    theta =  (273 + next_std_temp) / (273 + 15);
                    snd_spd_temp = 661 * math.sqrt(theta);
                    var clb_tas = clb_ias + clb_ias * (next_opm_alt/600/100);
                    mach_conv = clb_tas / snd_spd_temp;
                    if (mach_conv > clb_mach) {
                        mach_conv = clb_mach;
                    }
                    thrust = 14750 * 2 * milthrust_inp(mach_conv, next_opm_alt);
                    cl_for_clb = (-(thrust/grs_wgt_crs) + math.sqrt(math.pow(thrust/grs_wgt_crs,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    if (cl_for_clb < 0 ) {
                        cl_for_clb = (-(thrust/grs_wgt_crs) - math.sqrt(math.pow(thrust/grs_wgt_crs,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    }
                    cd_for_clb = cd0 + ind_drag_K * math.pow(cl_for_clb, 2);
                    sin_ang_clb = thrust/grs_wgt_crs - cd_for_clb/cl_for_clb;
                    if (mach_conv < clb_mach) {
                        vrt_spd_toc =  clb_tas * sin_ang_clb * 101.2685;
                    } else {
                        var clb_tas = mach_conv * snd_spd_temp;
                        vrt_spd_toc = clb_tas * sin_ang_clb * 101.2685;
                    }
                    crs_min_count = crs_min_count + 1;
                    rte_gone = rte_gone + (1/60 * crs_tas);
                    ete_gone = ete_gone + (1/60);
                }

                if (vrt_spd_toc > 500) {
                    var last_opm_alt = opm_alt;
                    opm_alt = next_opm_alt - 2000;

                    var vrt_spd_next_step = -999999;
                    var grs_wgt_next_step = grs_wgt_crs;

                    for (i=0; i<8; i=i+1) {
                        std_temp = 15 - 1.98 * (opm_alt/1000);
                        theta =  (273 + std_temp) / (273 + 15);
                        snd_spd_temp = 661 * math.sqrt(theta);
                        var clb_tas = clb_ias + clb_ias * (opm_alt/600/100);
                        mach_conv = clb_tas / snd_spd_temp;
                        if (mach_conv > clb_mach) {
                            mach_conv = clb_mach;
                        }
                        thrust = 14750 * 2 * milthrust_inp(mach_conv, opm_alt);
                        cl_for_clb = (-(thrust/grs_wgt_next_step) + math.sqrt(math.pow(thrust/grs_wgt_next_step,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                        if (cl_for_clb < 0 ) {
                            cl_for_clb = (-(thrust/grs_wgt_next_step) - math.sqrt(math.pow(thrust/grs_wgt_next_step,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                        }
                        cd_for_clb = cd0 + ind_drag_K * math.pow(cl_for_clb, 2);
                        sin_ang_clb = thrust/grs_wgt_next_step - cd_for_clb/cl_for_clb;
                        if (mach_conv < clb_mach) {
                            vrt_spd_next_step =  clb_tas * sin_ang_clb * 101.2685;
                        } else {
                            var clb_tas = mach_conv * snd_spd_temp;
                            vrt_spd_next_step = clb_tas * sin_ang_clb * 101.2685;
                        }
                        var clb_time_toc = 0;
                        if ((abs_alt_lmt - last_opm_alt) / (abs_alt_lmt - opm_alt) != 1) {
                            abs_alt_lmt = (last_opm_alt * vrt_spd_next_step - opm_alt * vrt_spd_toc) / (vrt_spd_next_step - vrt_spd_toc);
                            clb_time_toc = (abs_alt_lmt / vrt_spd_toc) * math.ln((abs_alt_lmt - last_opm_alt) / (abs_alt_lmt - opm_alt));
                        }
                        clb_time_toc = clb_time_toc / 60;
                        var fuel_burned_toc = clb_time_toc * thrust * 0.623;
                        grs_wgt_next_step = grs_wgt_crs - fuel_burned_toc;
                    }
                    rte_gone = rte_gone + (clb_time_toc * clb_tas);
                    ete_gone = ete_gone + clb_time_toc;
                    grs_wgt_crs = grs_wgt_next_step;
                }
            }

            var grs_wgt_hold = grs_wgt_crs;

            for (hold_min=0; hold_min<=35; hold_min=hold_min+1) {
                var hold_std_temp = 15 - 1.98 * (5000 / 1000);
                var hold_tas = 200 * (1 + (5000/600/100));
                var hold_thr = 14750 * 2 * milthrust_inp(mach_conv, 5000);
                var sin_ang_hold = 99999;
                while(sin_ang_hold>0.008){
                    hold_thr = hold_thr - 100;
                    hold_cl = (-(hold_thr/grs_wgt_hold) + math.sqrt(math.pow(hold_thr/grs_wgt_hold,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    if (hold_cl < 0 ) {
                        hold_cl = (-(hold_thr/grs_wgt_hold) - math.sqrt(math.pow(hold_thr/grs_wgt_hold,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
                    }
                    hold_cd = cd0 + ind_drag_K * math.pow(hold_cl, 2);
                    sin_ang_hold = hold_thr/grs_wgt_hold - hold_cd/hold_cl;
                }
                var hold_fuel_rate = (hold_thr * 0.623);
                var hold_fuel_brn_min = hold_fuel_rate / 60;
                grs_wgt_hold = grs_wgt_hold - hold_fuel_brn_min;
            }
            ete_gone = ete_gone + (35/60);

            var grs_wgt_ld = grs_wgt_hold;
            ttl_fuel_prd = grs_wgt_to - grs_wgt_ld + to_fuel + ldg_fuel;    
            grs_wgt_to = zfw + ttl_fuel_prd;
            if (grs_wgt_to > 98000) {
                grs_wgt_to = 98000;
            }
        }
        altn_fuel_prd = ttl_fuel_prd;
        ete_to_altn = ete_gone;
    }

    var fuel_opm_alt_prd = func() {
        fuel_opm_alt_dest();
        fuel_opm_alt_altn();
    }

    var vy_finder = func(test_alt, grs_wgt){
        var crn_dns_alt = getprop("/fdm/jsbsim/atmosphere/density-altitude");
        var crn_dns_air = getprop("/environment/density-slugft3");
        var crn_temp = getprop("/environment/temperature-degc");
        var std_temp = 15 - 1.98 * (test_alt/1000);
        var fd_temp = crn_temp - ((test_alt - crn_dns_alt) / 1000 * 1.98);
        var theta =  (273 + fd_temp) / (273 + 15);
        var snd_spd_temp = 661 * math.sqrt(theta);
        var oswald_efc = 0.755877483;
        var ind_drag_K = 1 / (oswald_efc * math.pi * 8.654260529);
        var cd0 = 0.013;
        if (test_alt < 36089) {
            dns_air = crn_dns_air - (0.000000046 * (test_alt - crn_dns_alt));
        } else {
            dns_air = crn_dns_air - (0.000000046 * (36089 - crn_dns_alt)) - (0.000000018 * (test_alt - 36089));
        }
        
        var vy_mach = 0.0;
        var vy_tas = 0.0;
        for (i=0; i<32; i=i+1) {
            var thrust = 14750 * 2 * milthrust_inp(vy_mach, test_alt);
            var cl_for_vy = (-(thrust/grs_wgt) + math.sqrt(math.pow(thrust/grs_wgt,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
            if (cl_for_vy < 0 ) {
                cl_for_vy = (-(thrust/grs_wgt) - math.sqrt(math.pow(thrust/grs_wgt,2) + 12 * cd0 * ind_drag_K)) / (2 * ind_drag_K);
            }
            var cd_for_vy = cd0 + ind_drag_K * math.pow(cl_for_vy, 2);
            var vy = math.sqrt(grs_wgt / (0.5 * dns_air * 1021 * cl_for_vy));
            vy_tas = vy * 0.592;
            var vy_ias = vy_tas / (1 + (test_alt/600/100 + (fd_temp - std_temp)/5/100));
            vy_mach = vy_tas / snd_spd_temp;
        }
        return(vy_ias);
    }

    var vspeed_calc = func() {
        var dns_air = getprop("/environment/density-slugft3") * 515.378819;
        var g_accl = getprop("/environment/gravitational-acceleration-mps2");
        var grs_wgt = getprop("/fdm/jsbsim/inertia/weight-lbs") / 2.20462262185;
        var wing_area = getprop("/fdm/jsbsim/metrics/Sw-sqft") * 0.092903;
        var slat_clad = getprop("/fdm/jsbsim/aero/alpha-deg") * 0.00303;
        var cln_clmax = 1.617;
        var flp6_clmax = 1.617 + 0.065 + 0.054 + 0.051 - 0.063 + slat_clad;
        var flp16_clmax = 1.617 + 0.171 + 0.142 + 0.134 - 0.063 + slat_clad;
        var flp30_clmax = 1.617 + 0.296 + 0.248 + 0.233 - 0.063 + slat_clad;
        cln_vso = math.sqrt((2*grs_wgt*g_accl)/(dns_air*wing_area*cln_clmax))/0.514444444444;
        flp6_vso = math.sqrt((2*grs_wgt*g_accl)/(dns_air*wing_area*flp6_clmax))/0.514444444444;
        flp16_vso = math.sqrt((2*grs_wgt*g_accl)/(dns_air*wing_area*flp16_clmax))/0.514444444444;
        flp30_vso = math.sqrt((2*grs_wgt*g_accl)/(dns_air*wing_area*flp30_clmax))/0.514444444444;
        cln_vref = cln_vso * 1.3;
        flp6_vref = flp6_vso * 1.3;
        flp16_vref = flp16_vso * 1.3;
        flp30_vref = flp30_vso * 1.3;
    }
    var vspeed_calc_timer = maketimer(3, vspeed_calc);
    vspeed_calc_timer.start();

    var dff_bearings = func(a, b) {
        var dist = math.fmod(a - b + 360, 360);
        if (dist > 180) {
            dist = 360 - dist;
        }
        return dist;
    }

    var read_crt_wpt = func(wpt_name, isr_num) {
        var navaid_dat_path = getprop("/sim/fg-aircraft") ~ "/bd700/Navaids/WPNAVAID.txt";
        var fix_dat_path = getprop("/sim/fg-aircraft") ~ "/bd700/Navaids/WPNAVFIX.txt";
        if (io.stat(navaid_dat_path) == nil) {
            var navaid_dat = " ";
        } else {
            var navaid_dat = io.readfile(navaid_dat_path);
        }
        if (io.stat(fix_dat_path) == nil) {
            var fix_dat = " ";
        } else {
            var fix_dat = io.readfile(fix_dat_path);
        }
        var navaid_dat_rmn = navaid_dat;
        var fix_dat_rmn = fix_dat;
        var wpt_name_len = size(wpt_name);
        var wpt_pos = 65532;
        var wpt_vec = [];
        var wpt_found_name = [];
        var wpt_found_lat = [];
        var wpt_found_lon = [];
        var wpt_found_dst = [];
        while (wpt_pos != -1) {
            wpt_pos = find(wpt_name, navaid_dat_rmn);
            var real_wpt_name = "";
            var w_c = 0;
            while(substr(navaid_dat_rmn, wpt_pos + w_c, 1) != " "){
                real_wpt_name = real_wpt_name ~ substr(navaid_dat_rmn, wpt_pos + w_c, 1);
                w_c = w_c + 1;
            }
            var lat_text = substr(navaid_dat_rmn, wpt_pos+9, 10);
            if (substr(lat_text, 0, 2) == "  "){
                var lat_num = num(substr(lat_text, 2, 8));
            } else if (substr(lat_text, 0, 1) == " ") {
                var lat_num = num(substr(lat_text, 1, 9));
            } else {
                var lat_num = num(lat_text);
            }
            var lon_text = substr(navaid_dat_rmn, wpt_pos+19, 11);
            if (substr(lon_text, 0, 3) == "   ") {
                var lon_num = num(substr(lon_text, 3, 8));
            } else if (substr(lon_text, 0, 2) == "  ") {
                var lon_num = num(substr(lon_text, 2, 9));
            } else if (substr(lon_text, 0, 1) == " ") {
                var lon_num = num(substr(lon_text, 1, 10));
            } else {
                var lon_num = num(lon_text);
            }
            if (lat_num != nil and lon_num != nil and real_wpt_name == wpt_name) {
                append(wpt_found_name, wpt_name);
                append(wpt_found_lat, lat_num);
                append(wpt_found_lon, lon_num);
            }
            navaid_dat_rmn = substr(navaid_dat_rmn, wpt_pos + wpt_name_len);
        }
        if (size(wpt_found_name) == 0) {
            wpt_pos = 65532;
        }
        while (wpt_pos != -1) {
            wpt_pos = find(wpt_name, fix_dat_rmn);
            var real_wpt_name = "";
            var w_c = 0;
            while(substr(fix_dat_rmn, wpt_pos + w_c, 1) != " "){
                real_wpt_name = real_wpt_name ~ substr(fix_dat_rmn, wpt_pos + w_c, 1);
                w_c = w_c + 1;
            }
            var lat_text = substr(fix_dat_rmn, wpt_pos+29, 10);
            if (substr(lat_text, 0, 2) == "  ") {
                var lat_num = num(substr(lat_text, 2, 8));
            } else if (substr(lat_text, 0, 1) == " ") {
                var lat_num = num(substr(lat_text, 1, 9));
            } else {
                var lat_num = num(lat_text);
            }
            var lon_text = substr(fix_dat_rmn, wpt_pos+39, 11);
            if (substr(lon_text, 0, 3) == "   ") {
                var lon_num = num(substr(lon_text, 3, 8));
            } else if (substr(lon_text, 0, 2) == "  ") {
                var lon_num = num(substr(lon_text, 2, 9));
            } else if (substr(lon_text, 0, 1) == " ") {
                var lon_num = num(substr(lon_text, 1, 10));
            } else {
                var lon_num = num(lon_text);
            }
            if (lat_num != nil and lon_num != nil and real_wpt_name == wpt_name) {
                append(wpt_found_name, wpt_name);
                append(wpt_found_lat, lat_num);
                append(wpt_found_lon, lon_num);
            }
            fix_dat_rmn = substr(fix_dat_rmn, wpt_pos + wpt_name_len);
        }
        if (size(wpt_found_name) == 0 and ((find("N", wpt_name) != -1 and find("E", wpt_name) != -1) or (find("N", wpt_name) != -1 and find("W", wpt_name) != -1) or (find("S", wpt_name) != -1 and find("E", wpt_name) != -1) or (find("S", wpt_name) != -1 and find("W", wpt_name) != -1))) {
            wc = 0;
            while((substr(wpt_name, 0, 1) == "N" or substr(wpt_name, 0, 1) == "S") and substr(wpt_name, wc, 1) != "E" and substr(wpt_name, wc, 1) != "W") {
                wc = wc + 1;
            }
            if (wc <= 1) {
                var inp_lat_deg = 9999;
                var inp_lat_min = 0.0;
            } else if (wc == 2) {
                var inp_lat_deg = num(substr(wpt_name, 1, 1)) or 9999;
                var inp_lat_min = 0.0;
            } else if (wc == 3) {
                var inp_lat_deg = num(substr(wpt_name, 1, 2)) or 9999;
                var inp_lat_min = 0.0;
            } else if (wc == 4) {
                var inp_lat_deg = num(substr(wpt_name, 1, 2)) or 9999;
                var inp_lat_min = (num(substr(wpt_name, 3, 1)) or 0) * 10;
            } else if (wc >= 5) {
                var inp_lat_deg = num(substr(wpt_name, 1, 2)) or 9999;
                var inp_lat_min = num(substr(wpt_name, 3, wc - 3)) or 0;
            }
            if (size(substr(wpt_name, wc)) <=1) {
                var inp_lon_deg = 9999;
                var inp_lon_min = 0.0;
            } else if (size(substr(wpt_name, wc)) == 2) {
                var inp_lon_deg = num(substr(wpt_name, wc + 1, 1)) or 9999;
                var inp_lon_min = 0;
            } else if (size(substr(wpt_name, wc)) == 3) {
                var inp_lon_deg = num(substr(wpt_name, wc + 1, 2)) or 9999;
                var inp_lon_min = 0;
            } else if (size(substr(wpt_name, wc)) == 4) {
                var inp_lon_deg = num(substr(wpt_name, wc + 1, 3)) or 9999;
                var inp_lon_min = 0;
            } else if (size(substr(wpt_name, wc)) == 5) {
                var inp_lon_deg = num(substr(wpt_name, wc + 1, 3)) or 9999;
                var inp_lon_min = (num(substr(wpt_name, wc + 4, 1)) or 0) * 10;
            } else if (size(substr(wpt_name, wc)) >= 6) {
                var inp_lon_deg = num(substr(wpt_name, wc + 1, 3)) or 9999;
                var inp_lon_min = num(substr(wpt_name, wc + 4)) or 0;
            }

            if (inp_lat_deg == 90 and inp_lat_min != 0) {
                scratch_pad_message = "INVALID INPUT";
            } else if (inp_lon_deg == 180 and inp_lon_min != 0) {
                scratch_pad_message = "INVALID INPUT";
            } else if (inp_lat_deg <= 90 and inp_lon_deg <= 180 and inp_lat_min < 60 and inp_lon_min < 60) {
                var cdn_dsp = "";
                cdn_dsp = cdn_dsp ~ substr(wpt_name, 0, 1) ~ sprintf("%02d", inp_lat_deg);
                cdn_dsp = cdn_dsp ~ substr(wpt_name, wc, 1) ~ sprintf("%03d", inp_lon_deg);
                if (substr(wpt_name, 0, 1) == "S") {
                    var inp_lat = -(inp_lat_deg + (inp_lat_min / 60));
                } else {
                    var inp_lat = inp_lat_deg + (inp_lat_min / 60);
                }
                if (substr(wpt_name, wc, 1) == "W") {
                    var inp_lon = -(inp_lon_deg + (inp_lon_min / 60));
                } else {
                    var inp_lon = inp_lon_deg + (inp_lon_min / 60);
                }
                var wp = createWaypoint.new(cdn_dsp, inp_lat, inp_lon);
                waypoints.insert(isr_num, wp);
                wpt_num = waypoints.size();
                rte_wpt_num = rte_wpt_counter();
                var wc = 0;
                while(waypoints.vector[wc].lat != inp_lat or (waypoints.vector[wc].lon != inp_lon) and wc <= wpt_num) {
                    wc = wc + 1;
                }
                if (wc < isr_num){
                    for (i=wc; i<(isr_num); i=i+1){
                        waypoints.pop(wc);
                    }
                }
                total_page_rte = math.ceil(rte_wpt_num / 3);
                total_page_prop = total_page_rte + total_page_miss + total_page_altn;
                if (current_page_prop >= total_page_prop) {
                    current_page_prop = total_page_prop;
                }
            } else {
                scratch_pad_message = "NOT IN DATABASE";
            }
        } else if (size(wpt_found_name) == 0){
            scratch_pad_message = "NOT IN DATABASE";
        } else {
            var pre_num = isr_num - 1;
            var pre_lat = waypoints.vector[pre_num].lat;
            var pre_lon = waypoints.vector[pre_num].lon;
            var ert_R = 6371000;
            var pre_lat_rad = pre_lat * D2R;
            var pre_lon_rad = pre_lon * D2R;
            for (i=0; i<size(wpt_found_name); i=i+1) {
                var wpt_lat_rad = wpt_found_lat[i] * D2R;
                var wpt_lon_rad = wpt_found_lon[i] * D2R;
                var delta_lat = wpt_lat_rad - pre_lat_rad;
                var delta_lon = wpt_lon_rad - pre_lon_rad;
                var a_cfn = math.sin(delta_lat/2) * math.sin(delta_lat/2) + 
                            math.cos(pre_lat_rad) * math.cos(wpt_lat_rad) *
                            math.sin(delta_lon/2) * math.sin(delta_lon/2);
                var c_cfn = 2 * math.atan2(math.sqrt(a_cfn), math.sqrt(1-a_cfn));
                var wpt_dst = ert_R * c_cfn / 1000 / 1.852;
                append(wpt_found_dst, wpt_dst);
            }
            setsize(wpt_vec, size(wpt_found_name));
            for (i=0; i<size(wpt_found_name); i=i+1) {
                wpt_vec[i] = [wpt_found_name[i], wpt_found_lat[i], wpt_found_lon[i], wpt_found_dst[i]];
            }
            var sort_rules = func(a, b){
                if (a[3] < b[3]) {
                    return -1;
                } else if (a[3] == b[3]) {
                    return 0;
                } else {
                    return 1;
                }
            }
            wpt_vec = sort(wpt_vec, sort_rules);
            wpt_sel_isr = isr_num;
            wpt_sel_vec = wpt_vec;
            if (size(wpt_vec) == 1) {
                var isr_lat = wpt_sel_vec[0][1];
                var isr_lon = wpt_sel_vec[0][2];
                var isr_wpt = createWaypoint.new(wpt_sel_vec[0][0], isr_lat, isr_lon);
                waypoints.insert(wpt_sel_isr, isr_wpt);
                wpt_num = waypoints.size();
                rte_wpt_num = rte_wpt_counter();
                var wc = 0;
                while((waypoints.vector[wc].lat != isr_lat or waypoints.vector[wc].lon != isr_lon) and wc <= wpt_num) {
                    wc = wc + 1;
                }
                if (wc <  wpt_sel_isr){
                    for (i=wc; i<(wpt_sel_isr); i=i+1){
                        waypoints.pop(wc);
                    }
                }
                total_page_rte = math.ceil(rte_wpt_num / 3);
                total_page_prop = total_page_rte + total_page_miss + total_page_altn;
                if (current_page_prop >= total_page_prop) {
                    current_page_prop = total_page_prop;
                }
            } else {
                save_current_page = current_page_prop;
                current_page_prop = 1;
                page_title_text = "WAYPOINT SELECT";
            }
        }
    }

    var awy_parser = func(via_to) {
        var awy_dat = getprop("/sim/fg-aircraft") ~ "/bd700/Navaids/WPNAVRTE.txt";
        if (io.stat(awy_dat) == nil) {
            var awy_dat_content ="I am angry!";
        } else {
            var awy_dat_content = io.readfile(awy_dat);
        }
        var awy_dat_remain = awy_dat_content;
        var awy_dat_size = size(awy_dat_content);
        awy_wpt_list = [];
        awy_wpt_list_lat = [];
        awy_wpt_list_lon = [];
        var tmp_list = [];
        var tmp_list_lat = [];
        var tmp_list_lon = [];
        var to_id = substr(via_to, (find(".",via_to)+1));
        var awy_id_len = find(".", via_to);
        var awy_id = substr(via_to, 0, awy_id_len);
        wpt_num = waypoints.size();
        var from_id = waypoints.vector[(wpt_num-1)].id;
        var awy_cursor = find(awy_id, awy_dat_remain);
        while (awy_cursor != -1) {
            var real_awy_id = "";
            if (substr(awy_dat_remain, awy_cursor - 1, 1) == chr(13) or awy_cursor == 0) {
                while(substr(awy_dat_remain, awy_cursor, 1) != " "){
                    real_awy_id = real_awy_id ~ substr(awy_dat_remain, awy_cursor, 1);
                    awy_cursor = awy_cursor + 1;
                }
            }
            if (awy_id == real_awy_id) {
                awy_cursor = awy_cursor + 5;
                var tmp_wpt_id = "";
                while(substr(awy_dat_remain, awy_cursor, 1) != " ") {
                    tmp_wpt_id = tmp_wpt_id ~ substr(awy_dat_remain, awy_cursor, 1);
                    awy_cursor = awy_cursor + 1;
                }
                awy_cursor = awy_cursor + 1;

                var tmp_wpt_lat = "";
                while(substr(awy_dat_remain, awy_cursor, 1) != " ") {
                    tmp_wpt_lat = tmp_wpt_lat ~ substr(awy_dat_remain, awy_cursor, 1);
                    awy_cursor = awy_cursor + 1;
                }

                tmp_wpt_lat = num(tmp_wpt_lat);

                awy_cursor = awy_cursor + 1;            
                var tmp_wpt_lon = "";
                while(substr(awy_dat_remain, awy_cursor, 1) != chr(13)) {
                    tmp_wpt_lon = tmp_wpt_lon ~ substr(awy_dat_remain, awy_cursor, 1);
                    awy_cursor = awy_cursor + 1;
                }
                tmp_wpt_lon = num(tmp_wpt_lon);

                append(tmp_list, tmp_wpt_id);
                append(tmp_list_lat, tmp_wpt_lat);
                append(tmp_list_lon, tmp_wpt_lon);
            }            
            awy_dat_remain = substr(awy_dat_remain, awy_cursor);
            awy_cursor = find(awy_id, awy_dat_remain);
        }

        if (size(tmp_list) == 0) {
            return 0;
        }

        var from_idx = 0;
        while(from_idx < size(tmp_list) and from_id != tmp_list[from_idx]) {
            from_idx = from_idx + 1;
        }
        var to_idx = 0;
        while(to_idx < size(tmp_list) and to_id != tmp_list[to_idx]){
            to_idx = to_idx + 1;
        }
        var awy_len = abs(from_idx - to_idx) + 1;

        if (from_id == tmp_list[from_idx] and to_id == tmp_list[to_idx] and from_idx != to_idx) {
            var wc = from_idx;
            while(wc <= to_idx) {
                append(awy_wpt_list, tmp_list[wc]);
                append(awy_wpt_list_lat, tmp_list_lat[wc]);
                append(awy_wpt_list_lon, tmp_list_lon[wc]);
                wc = wc + 1;
            }
            var wc = from_idx;
            while(wc >= to_idx) {
                append(awy_wpt_list, tmp_list[wc]);
                append(awy_wpt_list_lat, tmp_list_lat[wc]);
                append(awy_wpt_list_lon, tmp_list_lon[wc]);
                wc = wc - 1;
            }
       
            return 1;
        } else {
            return 0;
        }
    }

    var grtCircHdg = func (wpt_idx) {
        var pre_lat = waypoints.vector[(wpt_idx - 1)].lat_nd * D2R;
        var pre_lon = waypoints.vector[(wpt_idx - 1)].lon_nd * D2R;
        var crt_lat = waypoints.vector[wpt_idx].lat_nd * D2R;
        var crt_lon = waypoints.vector[wpt_idx].lon_nd * D2R;
        var y_par = math.sin(crt_lon - pre_lon) * math.cos(crt_lat);
        var x_par = math.cos(pre_lat) * math.sin(crt_lat) - 
                    math.sin(pre_lat) * math.cos(crt_lat) * 
                    math.cos(crt_lon - pre_lon);
        var brng = math.atan2(y_par, x_par) * R2D;
        brng = math.fmod((brng + 360), 360);
        return (brng);
    }

    var grtCircDist = func (wpt_idx) {
        var ert_r = 6371000;
        var pre_lat = waypoints.vector[(wpt_idx - 1)].lat_nd * D2R;
        var pre_lon = waypoints.vector[(wpt_idx - 1)].lon_nd * D2R;
        var crt_lat = waypoints.vector[wpt_idx].lat_nd * D2R;
        var crt_lon = waypoints.vector[wpt_idx].lon_nd * D2R;
        var delta_lat = crt_lat - pre_lat;
        var delta_lon = crt_lon - pre_lon;
        var a_par = math.sin(delta_lat / 2) * math.sin(delta_lat / 2) +
                    math.cos(pre_lat) * math.cos(crt_lat) *
                    math.sin(delta_lon / 2) * math.sin(delta_lon / 2);
        var c_par = 2 * math.atan2(math.sqrt(a_par), math.sqrt(1 - a_par));
        var dist = ert_r * c_par / 1000 * 0.54;
        return (dist);
    }

    var hdg_frm_acr = func (wpt_idx) {
        var pre_lat = getprop("/position/latitude-deg") or 0;
        var pre_lon = getprop("/position/longitude-deg") or 0;
        pre_lat = pre_lat * D2R;
        pre_lon = pre_lon * D2R;
        var crt_lat = waypoints.vector[wpt_idx].lat_nd * D2R;
        var crt_lon = waypoints.vector[wpt_idx].lon_nd * D2R;
        var y_par = math.sin(crt_lon - pre_lon) * math.cos(crt_lat);
        var x_par = math.cos(pre_lat) * math.sin(crt_lat) - 
                    math.sin(pre_lat) * math.cos(crt_lat) * 
                    math.cos(crt_lon - pre_lon);
        var brng = math.atan2(y_par, x_par) * R2D;
        brng = math.fmod((brng + 360), 360);
        return (brng);
    }

    var dst_frm_acr = func (wpt_idx) {
        var ert_r = 6371000;
        var pre_lat = getprop("/position/latitude-deg") or 0;
        var pre_lon = getprop("/position/longitude-deg") or 0;
        pre_lat = pre_lat * D2R;
        pre_lon = pre_lon * D2R;
        var crt_lat = waypoints.vector[wpt_idx].lat_nd * D2R;
        var crt_lon = waypoints.vector[wpt_idx].lon_nd * D2R;
        var delta_lat = crt_lat - pre_lat;
        var delta_lon = crt_lon - pre_lon;
        var a_par = math.sin(delta_lat / 2) * math.sin(delta_lat / 2) +
                    math.cos(pre_lat) * math.cos(crt_lat) *
                    math.sin(delta_lon / 2) * math.sin(delta_lon / 2);
        var c_par = 2 * math.atan2(math.sqrt(a_par), math.sqrt(1 - a_par));
        var dist = ert_r * c_par / 1000 * 0.54;
        return (dist);
    }

    var listSids = func(origin, rwy) {
        var rwy_sid_list = [];
        var sids_file = getprop("/sim/fg-aircraft") ~ "/bd700/Navaids/" ~ origin ~ ".xml";
        if (io.stat(sids_file) == nil) {
            var sids_cnt = "Too young to simple, sometime naive!";
        } else {
            var sids_cnt = io.readfile(sids_file);
        }
        var sids_cnt_rmn = sids_cnt;
        var sid_name_pos = 0;
        while (sid_name_pos != -1) {
            sid_name_pos = find('<Sid Name="', sids_cnt_rmn);
            var sid_name_bgn = sid_name_pos + 11;
            var sid_name_chr = '';
            var sid_name = "";
            var w_c  = 0;
            while(sid_name_chr != '"' and sid_name_pos != -1) {
                sid_name = sid_name ~ sid_name_chr;
                sid_name_chr = substr(sids_cnt_rmn, (sid_name_bgn + w_c), 1);
                w_c = w_c + 1;
            }
            sids_cnt_rmn = substr(sids_cnt_rmn, sid_name_bgn);
            var rwy_name_pos = find('Runways="', sids_cnt_rmn);
            var rwy_name_bgn = rwy_name_pos + size('Runways="');
            var rwy_name_chr = '';
            var rwy_name = "";
            var w_c = 0;
            while (rwy_name_chr != '"' and rwy_name_pos != -1) {
                rwy_name = rwy_name ~ rwy_name_chr;
                rwy_name_chr = substr(sids_cnt_rmn, (rwy_name_bgn + w_c) ,1);
                w_c = w_c + 1;
            }
            if (rwy_name == rwy) {
                append(rwy_sid_list, sid_name);
            }
        }
        return rwy_sid_list;
    }

    var isrSids = func (origin, sid_name){
        var sids_file = getprop("/sim/fg-aircraft") ~ "/bd700/Navaids/" ~ origin ~ ".xml";
        if (io.stat(sids_file) == nil) {
            var sids_cnt = "Exciting!";
        } else {
            var sids_cnt = io.readfile(sids_file);
        }
        var alt_vec = [0];
        var alt_rtt_vec = ["aa"];
        var spd_vec = [0];
        var wpt_type_vec = ["aa"];
        var fly_type_vec = ["aa"];
        var sid_blk = sids_cnt;
        var sid_name_pos = find(sid_name, sid_blk);
        sid_blk = substr(sid_blk, sid_name_pos);
        var sid_blk_bgn = find('<Sid_Waypoint ID="1">', sid_blk);
        var sid_blk_end = find("</Sid>", sid_blk);
        var sid_blk_size = sid_blk_end - sid_blk_bgn;
        sid_blk = substr(sid_blk, sid_blk_bgn, sid_blk_size);
        wpt_num = waypoints.size();
        var have_sid = 0;
        for (i=(wpt_num - 1); i>=0; i=i-1) {
            var proc_type = waypoints.vector[i].procedure;
            if (proc_type == "sid") {
                waypoints.pop(i);
            }
        }

        var find_cnt = 0;
        for (i=(sid_blk_size-size('<Sid_Waypoint ID="')); i>=0; i=i-1) {
            var srh_rst = substr(sid_blk, i, size('<Sid_Waypoint ID="'));
            if (srh_rst == '<Sid_Waypoint ID="') {
                var sid_wpt_blk = substr(sid_blk, i);
                var name = substr(sid_wpt_blk, (find("<Name>", sid_wpt_blk)+size("<Name>")), (find("</Name>", sid_wpt_blk) - (find("<Name>", sid_wpt_blk)+size("<Name>"))));
                var alt = substr(sid_wpt_blk, (find("<Altitude>", sid_wpt_blk)+size("<Altitude>")), (find("</Altitude>", sid_wpt_blk) - (find("<Altitude>", sid_wpt_blk)+size("<Altitude>"))));
                alt = num(alt);
                var alt_rtt = substr(sid_wpt_blk, (find("<AltitudeRestriction>", sid_wpt_blk)+size("<AltitudeRestriction>")), (find("</AltitudeRestriction>", sid_wpt_blk) - (find("<AltitudeRestriction>", sid_wpt_blk)+size("<AltitudeRestriction>"))));
                var spd = substr(sid_wpt_blk, (find("<Speed>", sid_wpt_blk)+size("<Speed>")), (find("</Speed>", sid_wpt_blk) - (find("<Speed>", sid_wpt_blk)+size("<Speed>"))));
                spd = num(spd);
                var wpt_lat = substr(sid_wpt_blk, (find("<Latitude>", sid_wpt_blk)+size("<Latitude>")), (find("</Latitude>", sid_wpt_blk) - (find("<Latitude>", sid_wpt_blk)+size("<Latitude>"))));
                wpt_lat = num(wpt_lat);
                var wpt_lon = substr(sid_wpt_blk, (find("<Longitude>", sid_wpt_blk)+size("<Longitude>")), (find("</Longitude>", sid_wpt_blk) - (find("<Longitude>", sid_wpt_blk)+size("<Longitude>"))));
                var wpt_lon = num(wpt_lon);
                var wpt_type = substr(sid_wpt_blk, (find("<Type>", sid_wpt_blk)+size("<Type>")), (find("</Type>", sid_wpt_blk) - (find("<Type>", sid_wpt_blk)+size("<Type>"))));
                var fly_type = substr(sid_wpt_blk, (find("<Flytype>", sid_wpt_blk)+size("<Flytype>")), (find("</Flytype>", sid_wpt_blk) - (find("<Flytype>", sid_wpt_blk)+size("<Flytype>"))));
                var alt_con = substr(sid_wpt_blk, (find("<AltitudeCons>", sid_wpt_blk)+size("<AltitudeCons>")), (find("</AltitudeCons>", sid_wpt_blk) - (find("<AltitudeCons>", sid_wpt_blk)+size("<AltitudeCons>"))));
                var hdg_crs = substr(sid_wpt_blk, (find("<Hdg_Crs>", sid_wpt_blk)+size("<Hdg_Crs>")), (find("</Hdg_Crs>", sid_wpt_blk) - (find("<Hdg_Crs>", sid_wpt_blk)+size("<Hdg_Crs>"))));
                var hdg_crs_value = substr(sid_wpt_blk, (find("<Hdg_Crs_value>", sid_wpt_blk)+size("<Hdg_Crs_value>")), (find("</Hdg_Crs_value>", sid_wpt_blk) - (find("<Hdg_Crs_value>", sid_wpt_blk)+size("<Hdg_Crs_value>"))));
                var bank_limit = substr(sid_wpt_blk, (find("<BankLimit>", sid_wpt_blk)+size("<BankLimit>")), (find("</BankLimit>", sid_wpt_blk) - (find("<BankLimit>", sid_wpt_blk)+size("<BankLimit>"))));
                var sp_turn = substr(sid_wpt_blk, (find("<Sp_Turn>", sid_wpt_blk)+size("<Sp_Turn>")), (find("</Sp_Turn>", sid_wpt_blk) - (find("<Sp_Turn>", sid_wpt_blk)+size("<Sp_Turn>"))));
                var radl_to_intc = substr(sid_wpt_blk, (find("<RadialtoIntercept>", sid_wpt_blk)+size("<RadialtoIntercept>")), (find("</RadialtoIntercept>", sid_wpt_blk) - (find("<RadialtoIntercept>", sid_wpt_blk)+size("<RadialtoIntercept>"))));
                var hld_rad_or_inbd = substr(sid_wpt_blk, (find("<Hld_Rad_or_Inbd>", sid_wpt_blk)+size("<Hld_Rad_or_Inbd>")), (find("</Hld_Rad_or_Inbd>", sid_wpt_blk) - (find("<Hld_Rad_or_Inbd>", sid_wpt_blk)+size("<Hld_Rad_or_Inbd>"))));
                var hld_rad_vlue = substr(sid_wpt_blk, (find("<Hld_Rad_value>", sid_wpt_blk)+size("<Hld_Rad_value>")), (find("</Hld_Rad_value>", sid_wpt_blk) - (find("<Hld_Rad_value>", sid_wpt_blk)+size("<Hld_Rad_value>"))));
                var hld_turn = substr(sid_wpt_blk, (find("<Hld_Turn>", sid_wpt_blk)+size("<Hld_Turn>")), (find("</Hld_Turn>", sid_wpt_blk) - (find("<Hld_Turn>", sid_wpt_blk)+size("<Hld_Turn>"))));
                var hld_time_dist = substr(sid_wpt_blk, (find("<Hld_Time_or_Dist>", sid_wpt_blk)+size("<Hld_Time_or_Dist>")), (find("</Hld_Time_or_Dist>", sid_wpt_blk) - (find("<Hld_Time_or_Dist>", sid_wpt_blk)+size("<Hld_Time_or_Dist>"))));
                var hld_td_value = substr(sid_wpt_blk, (find("<Hld_td_value>", sid_wpt_blk)+size("<Hld_td_value>")), (find("</Hld_td_value>", sid_wpt_blk) - (find("<Hld_td_value>", sid_wpt_blk)+size("<Hld_td_value>"))));

                var dme_to_intercept = substr(sid_wpt_blk, (find("<DMEtoIntercept>", sid_wpt_blk)+size("<DMEtoIntercept>")), (find("</DMEtoIntercept>", sid_wpt_blk) - (find("<DMEtoIntercept>", sid_wpt_blk)+size("<DMEtoIntercept>"))));

                if (waypoints.size() >= 2) {
                    var first_wpt = waypoints.vector[1].id;
                } else {
                    var first_wpt = "";
                }

                if (find_cnt == 0 and first_wpt == name) {
                    waypoints.pop(1);
                }
                var sid_wp = createWaypoint.new(name, wpt_lat, wpt_lon);
                sid_wp.type = wpt_type;
                sid_wp.speed = spd;
                sid_wp.altitude = alt;
                sid_wp.altitudeCons = alt_con;
                sid_wp.altitudeRestriction = alt_rtt;
                sid_wp.hdg_crs = hdg_crs;
                sid_wp.hdg_crs_value = hdg_crs_value;
                sid_wp.flytype = fly_type;
                sid_wp.bankLimit = bank_limit;
                sid_wp.radialtoIntercept = radl_to_intc;
                sid_wp.DMEtoIntercept = dme_to_intercept;
                sid_wp.sp_turn = sp_turn;
                sid_wp.hld_rad_or_inbd = hld_rad_or_inbd;
                sid_wp.hld_rad_vlue = hld_rad_vlue;
                sid_wp.hld_turn = hld_turn;
                sid_wp.hld_time_dist = hld_time_dist;
                sid_wp.hld_td_value = hld_td_value;
                sid_wp.procedure = "sid";
                waypoints.insert(1, sid_wp);

                find_cnt = find_cnt + 1;
            }
        }
        sel_sid = sid_name;
        var find_sid_bgn = '<Sid Name="' ~ sel_sid ~ '"';
        var sid_bgn = find(find_sid_bgn, sids_cnt);
        var sid_rst = substr(sids_cnt, sid_bgn);
        var sid_end = find("</Sid>", sid_rst);
        sid_end = sid_end + size("</Sid>");
        var sid_blk = substr(sid_rst, 0, sid_end);
        sel_sid_blk = sid_blk;
    }

    var listSidTran = func () {
        var sid_trn_list = [];
        var sid_blk_rmn = sel_sid_blk;
        while (size(sid_blk_rmn) > 0) {
            sid_trn_bgn = find('<Sid_Transition Name="', sid_blk_rmn);
            if (sid_trn_bgn == -1) {
                sid_blk_rmn = "";
            } else {
                sid_trn_bgn = sid_trn_bgn + size('<Sid_Transition Name="');
                sid_blk_rmn = substr(sid_blk_rmn, sid_trn_bgn);
                var trn_nam_siz = find('">', sid_blk_rmn);
                var trn_nam = substr(sid_blk_rmn, 0, trn_nam_siz);
                append(sid_trn_list, trn_nam);
            }
        }
        return(sid_trn_list);
    }

    var isrSidTran = func (tran_name) {
        var fnd_trn_nam = '<Sid_Transition Name="' ~ tran_name ~ '">';
        var sid_trn_bgn = find(fnd_trn_nam, sel_sid_blk);
        var sid_trn_rst = substr(sel_sid_blk, sid_trn_bgn);
        var sid_trn_end = find("</Sid_Transition>", sid_trn_rst);
        sid_trn_end = sid_trn_end + size("</Sid_Transition>");
        var sid_trn_blk = substr(sid_trn_rst, 0, sid_trn_end);
        var trn_blk_rmn = sid_trn_blk;
        var wpt_siz = waypoints.size();

        for (i=(wpt_siz - 1); i>=0; i=i-1) {
            var proc_type = waypoints.vector[i].procedure;
            if (proc_type == "sidtran") {
                waypoints.pop(i);
            }
        }

        var wpt_siz = waypoints.size();
        var found_last_sid = 0;
        var last_sid = (wpt_siz - 1);
        while (found_last_sid == 0 and last_sid >= 0) {
            var proc_type = waypoints.vector[last_sid].procedure;
            if (proc_type == "sid") {
                found_last_sid = 1;
            } else {
                last_sid = last_sid - 1;
            }
        }
        var while_find_counter = 0;
        while (size(trn_blk_rmn) > 0) {
            if (find('<SidTr_Waypoint ID="', trn_blk_rmn) == -1) {
                trn_blk_rmn = "";
            } else {
                while_find_counter = while_find_counter + 1;
                
                var tran_wpt_bgn = find('<SidTr_Waypoint ID="', trn_blk_rmn);
                tran_wpt_bgn = tran_wpt_bgn + size('<SidTr_Waypoint ID="');
                trn_blk_rmn = substr(trn_blk_rmn, tran_wpt_bgn);

                var wpt_nam_bgn = find("<Name>", trn_blk_rmn);
                wpt_nam_bgn = wpt_nam_bgn + size("<Name>");
                var wpt_nam_end = find("</Name>", trn_blk_rmn);
                var wpt_nam_len = wpt_nam_end - wpt_nam_bgn;
                var wpt_nam = substr(trn_blk_rmn, wpt_nam_bgn, wpt_nam_len);

                var wpt_typ_bgn = find("<Type>", trn_blk_rmn);
                wpt_typ_bgn = wpt_typ_bgn + size("<Type>");
                var wpt_typ_end = find("</Type>", trn_blk_rmn);
                var wpt_typ_len = wpt_typ_end - wpt_typ_bgn;
                var wpt_typ = substr(trn_blk_rmn, wpt_typ_bgn, wpt_typ_len);

                var wpt_lat_bgn = find("<Latitude>", trn_blk_rmn);
                wpt_lat_bgn = wpt_lat_bgn + size("<Latitude>");
                var wpt_lat_end = find("</Latitude>", trn_blk_rmn);
                var wpt_lat_len = wpt_lat_end - wpt_lat_bgn;
                var wpt_lat = substr(trn_blk_rmn, wpt_lat_bgn, wpt_lat_len);
                wpt_lat = num(wpt_lat);

                var wpt_lon_bgn = find("<Longitude>", trn_blk_rmn);
                wpt_lon_bgn = wpt_lon_bgn + size("<Longitude>");
                var wpt_lon_end = find("</Longitude>", trn_blk_rmn);
                var wpt_lon_len = wpt_lon_end - wpt_lon_bgn;
                var wpt_lon = substr(trn_blk_rmn, wpt_lon_bgn, wpt_lon_len);
                wpt_lon = num(wpt_lon);

                var wpt_spd_bgn = find("<Speed>", trn_blk_rmn);
                wpt_spd_bgn = wpt_spd_bgn + size("<Speed>");
                var wpt_spd_end = find("</Speed>", trn_blk_rmn);
                var wpt_spd_len = wpt_spd_end - wpt_spd_bgn;
                var wpt_spd = substr(trn_blk_rmn, wpt_spd_bgn, wpt_spd_len);
                wpt_spd = num(wpt_spd);
                
                var wpt_alt_bgn = find("<Altitude>", trn_blk_rmn);
                wpt_alt_bgn = wpt_alt_bgn + size("<Altitude>");
                var wpt_alt_end = find("</Altitude>", trn_blk_rmn);
                var wpt_alt_len = wpt_alt_end - wpt_alt_bgn;
                var wpt_alt = substr(trn_blk_rmn, wpt_alt_bgn, wpt_alt_len);
                wpt_alt = num(wpt_alt);

                var wpt_altcon_bgn = find("<AltitudeCons>", trn_blk_rmn);
                wpt_altcon_bgn = wpt_altcon_bgn + size("<AltitudeCons>");
                var wpt_altcon_end = find("</AltitudeCons>", trn_blk_rmn);
                var wpt_altcon_len = wpt_altcon_end - wpt_altcon_bgn;
                var wpt_altcon = substr(trn_blk_rmn, wpt_altcon_bgn, wpt_altcon_len);
                wpt_altcon = num(wpt_altcon);

                var wpt_altrsn_bgn = find("<AltitudeRestriction>", trn_blk_rmn);
                wpt_altrsn_bgn = wpt_altrsn_bgn + size("<AltitudeRestriction>");
                var wpt_altrsn_end = find("</AltitudeRestriction>", trn_blk_rmn);
                var wpt_altrsn_len = wpt_altrsn_end - wpt_altrsn_bgn;
                var wpt_altrsn = substr(trn_blk_rmn, wpt_altrsn_bgn, wpt_altrsn_len);
                wpt_altrsn = num(wpt_altrsn);

                var wpt_flytype_bgn = find("<Flytype>", trn_blk_rmn);
                wpt_flytype_bgn = wpt_flytype_bgn + size("<Flytype>");
                var wpt_flytype_end = find("</Flytype>", trn_blk_rmn);
                var wpt_flytype_len = wpt_flytype_end - wpt_flytype_bgn;
                var wpt_flytype = substr(trn_blk_rmn, wpt_flytype_bgn, wpt_flytype_len);

                var wpt_bnklmt_bgn = find("<BankLimit>", trn_blk_rmn);
                wpt_bnklmt_bgn = wpt_bnklmt_bgn + size("<BankLimit>");
                var wpt_bnklmt_end = find("</BankLimit>", trn_blk_rmn);
                var wpt_bnklmt_len = wpt_bnklmt_end - wpt_bnklmt_bgn;
                var wpt_bnklmt = substr(trn_blk_rmn, wpt_bnklmt_bgn, wpt_bnklmt_len);
                wpt_bnklmt = num(wpt_bnklmt);

                var wpt_spturn_bgn = find("<Sp_Turn>", trn_blk_rmn);
                wpt_spturn_bgn = wpt_spturn_bgn + size("<Sp_Turn>");
                var wpt_spturn_end = find("</Sp_Turn>", trn_blk_rmn);
                var wpt_spturn_len = wpt_spturn_end - wpt_spturn_bgn;
                var wpt_spturn = substr(trn_blk_rmn, wpt_spturn_bgn, wpt_spturn_len);

                if (while_find_counter == 1) {
                    var wp_siz = waypoints.size();
                    var last_wp_id = waypoints.vector[(wp_siz - 1)].id;
                    if (last_wp_id == wpt_nam) {
                        waypoints.pop(wp_siz - 1);
                    }
                }

                var trn_wp_hsh = createWaypoint.new(wpt_nam, wpt_lat, wpt_lon);
                trn_wp_hsh.type = wpt_typ;
                trn_wp_hsh.speed = wpt_spd;
                trn_wp_hsh.altitude = wpt_alt;
                trn_wp_hsh.altitudeCons = wpt_altcon;
                trn_wp_hsh.altitudeRestriction = wpt_altrsn;
                trn_wp_hsh.flytype = wpt_flytype;
                trn_wp_hsh.bankLimit = wpt_bnklmt;
                trn_wp_hsh.sp_turn = wpt_spturn;
                trn_wp_hsh.procedure = "sidtran";

                var trn_wp_ins = last_sid + while_find_counter;
                waypoints.insert(trn_wp_ins, trn_wp_hsh);
                sel_sid_tran = tran_name;
            }
        }
    }

    var fmzInput = func {
        key_prs("A", func(){
            scratch_pad_text = scratch_pad_text ~ "A";
        });
        if (B_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-B-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "B";
            }
        }
        B_press_orig = getprop("/instrumentation/fmz/fmz-key-B-press");

        if (C_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-C-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "C";
            }
        }
        C_press_orig = getprop("/instrumentation/fmz/fmz-key-C-press");

        if (D_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-D-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "D";
            }
        }
        D_press_orig = getprop("/instrumentation/fmz/fmz-key-D-press");

        if (E_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-E-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "E";
            }
        }
        E_press_orig = getprop("/instrumentation/fmz/fmz-key-E-press");

        if (F_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-F-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "F";
            }
        }
        F_press_orig = getprop("/instrumentation/fmz/fmz-key-F-press");

        if (G_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-G-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "G";
            }
        }
        G_press_orig = getprop("/instrumentation/fmz/fmz-key-G-press");

        if (H_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-H-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "H";
            }
        }
        H_press_orig = getprop("/instrumentation/fmz/fmz-key-H-press");

        if (I_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-I-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "I";
            }
        }
        I_press_orig = getprop("/instrumentation/fmz/fmz-key-I-press");

        if (J_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-J-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "J";
            }
        }
        J_press_orig = getprop("/instrumentation/fmz/fmz-key-J-press");

        if (K_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-K-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "K";
            }
        }
        K_press_orig = getprop("/instrumentation/fmz/fmz-key-K-press");

        if (L_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-L-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "L";
            }
        }
        L_press_orig = getprop("/instrumentation/fmz/fmz-key-L-press");

        if (M_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-M-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "M";
            }
        }
        M_press_orig = getprop("/instrumentation/fmz/fmz-key-M-press");

        if (N_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-N-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "N";
            }
        }
        N_press_orig = getprop("/instrumentation/fmz/fmz-key-N-press");

        if (O_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-O-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "O";
            }
        }
        O_press_orig = getprop("/instrumentation/fmz/fmz-key-O-press");

        if (P_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-P-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "P";
            }
        }
        P_press_orig = getprop("/instrumentation/fmz/fmz-key-P-press");

        if (Q_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-Q-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "Q";
            }
        }
        Q_press_orig = getprop("/instrumentation/fmz/fmz-key-Q-press");

        if (R_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-R-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "R";
            }
        }
        R_press_orig = getprop("/instrumentation/fmz/fmz-key-R-press");

        if (S_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-S-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "S";
            }
        }
        S_press_orig = getprop("/instrumentation/fmz/fmz-key-S-press");

        if (T_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-T-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "T";
            }
        }
        T_press_orig = getprop("/instrumentation/fmz/fmz-key-T-press");

        if (U_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-U-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "U";
            }
        }
        U_press_orig = getprop("/instrumentation/fmz/fmz-key-U-press");

        if (V_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-V-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "V";
            }
        }
        V_press_orig = getprop("/instrumentation/fmz/fmz-key-V-press");

        if (W_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-W-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "W";
            }
        }
        W_press_orig = getprop("/instrumentation/fmz/fmz-key-W-press");

        if (X_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-X-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "X";
            }
        }
        X_press_orig = getprop("/instrumentation/fmz/fmz-key-X-press");

        if (Y_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-Y-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "Y";
            }
        }
        Y_press_orig = getprop("/instrumentation/fmz/fmz-key-Y-press");

        if (Z_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-Z-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "Z";
            }
        }
        Z_press_orig = getprop("/instrumentation/fmz/fmz-key-Z-press");

        if (DEL_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-DEL-press");
            if (press == 1 and scratch_pad_message == "") {
                scratch_pad_message = "DELETE";
            }
        }
        DEL_press_orig = getprop("/instrumentation/fmz/fmz-key-DEL-press");

        if (CLR_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-CLR-press");
            if (press == 1) {
                scratch_pad_message = "";
                scratch_pad_text = substr(scratch_pad_text,0,size(scratch_pad_text)-1);
            }
        }
        CLR_press_orig = getprop("/instrumentation/fmz/fmz-key-CLR-press");
        var CLR_timer = maketimer(1, func(){
            if (CLR_press_orig == 1) {
                scratch_pad_text = "";
            }
        });
        CLR_timer.singleShot = 1;
        CLR_timer.start();

        if (num1_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-1-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "1";
            }
        }
        num1_press_orig = getprop("/instrumentation/fmz/fmz-key-1-press");

        if (num2_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-2-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "2";
            }
        }
        num2_press_orig = getprop("/instrumentation/fmz/fmz-key-2-press");

        if (num3_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-3-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "3";
            }
        }
        num3_press_orig = getprop("/instrumentation/fmz/fmz-key-3-press");

        if (num4_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-4-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "4";
            }
        }
        num4_press_orig = getprop("/instrumentation/fmz/fmz-key-4-press");

        if (num5_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-5-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "5";
            }
        }
        num5_press_orig = getprop("/instrumentation/fmz/fmz-key-5-press");

        if (num6_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-6-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "6";
            }
        }
        num6_press_orig = getprop("/instrumentation/fmz/fmz-key-6-press");

        if (num7_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-7-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "7";
            }
        }
        num7_press_orig = getprop("/instrumentation/fmz/fmz-key-7-press");

        if (num8_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-8-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "8";
            }
        }
        num8_press_orig = getprop("/instrumentation/fmz/fmz-key-8-press");

        if (num9_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-9-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "9";
            }
        }
        num9_press_orig = getprop("/instrumentation/fmz/fmz-key-9-press");

        if (num0_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-0-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "0";
            }
        }
        num0_press_orig = getprop("/instrumentation/fmz/fmz-key-0-press");

        if (DOT_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-DOT-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ ".";
            }
        }
        DOT_press_orig = getprop("/instrumentation/fmz/fmz-key-DOT-press");

        if (DASH_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-DASH-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "-";
            }
        }
        DASH_press_orig = getprop("/instrumentation/fmz/fmz-key-DASH-press");

        if (SLASH_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-SLASH-press");
            if (press == 1) {
                scratch_pad_text = scratch_pad_text ~ "/";
            }
        }
        SLASH_press_orig = getprop("/instrumentation/fmz/fmz-key-SLASH-press");

        if (PREV_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-PREV-press");
            if (press == 1) {
                current_page_prop = current_page_prop - 1;
                if (current_page_prop <= 1){
                    current_page_prop = 1;
                }
            }
        }
        PREV_press_orig = getprop("/instrumentation/fmz/fmz-key-PREV-press");
        
        if (NEXT_press_orig == 0) {
            var press = getprop("/instrumentation/fmz/fmz-key-NEXT-press");
            if (press == 1) {
                current_page_prop = current_page_prop + 1;
                if (current_page_prop >= total_page_prop){
                    current_page_prop = total_page_prop;
                }
            }
        }
        NEXT_press_orig = getprop("/instrumentation/fmz/fmz-key-NEXT-press");

        if (scratch_pad_message != ""){
            scratch_pad_text = "";
            scratch_pad_display = scratch_pad_message;
        } else if (scratch_pad_text != "") {
            scratch_pad_display = "[ " ~ scratch_pad_text ~ " ]";
        } else {
            scratch_pad_display = "";
        }
        scratchPad.setText(sprintf("%s", scratch_pad_display));
    }
    fmz_setlistener(fmzInput);

    var fmzClock = maketimer(1, func() {
        var time_set = global_set_time;
        if (time_set != time_orig) {
            time_counter = 0.0;
            time_1 = chr(time_set[3]);
            time_10 = chr(time_set[2]);
            time_100 = chr(time_set[1]);
            time_1000 = chr(time_set[0]);
        } else {
            time_counter = time_counter + 1;
        }

        if (time_counter != 0 and math.fmod(time_counter,60) == 0){
            time_1 = time_1 + 1;
        }
        if (time_1 >= 10) {
            time_10 = time_10 + 1;
            time_1 = 0;
        }
        if (time_10 >= 6) {
            time_100 = time_100 + 1;
            time_10 = 0;
        }
        if (time_100 >= 10) {
            time_1000 = time_1000 + 1;
            time_100 = 0;
        }
        if (time_1000 >= 2 and time_100 >= 4) {
            time_1000 = 0;
            time_100 = 0;
        }
        time_text = time_1000 ~ time_100 ~ time_10 ~ time_1 ~ "Z";
        
        
        time_orig = global_set_time;
    
    });
    fmzClock.start();

    var to_waypoint = 0;
    var from_waypoint = to_waypoint - 1;
    var to_wp_dst = [0,0];
    var to_wp_dst_switch = -1;
    var to_waypoint_check = maketimer(3,  func() {
        if (to_wp_dst_switch == -1) {
            to_wp_dst[0] = dst_frm_acr(to_waypoint);
        } else if (to_wp_dst_switch == 1) {
            to_wp_dst[1] = dst_frm_acr(to_waypoint);
        }
        
        if (to_wp_dst[1] > to_wp_dst[0]) {
            to_waypoint = to_waypoint + 1;
            from_waypoint = to_waypoint - 1;
        }

        if (to_waypoint >= waypoints.size()) {
            to_waypoint = waypoints.size() - 1;
        }

        to_wp_dst_switch = to_wp_dst_switch * (-1);
    });
    to_waypoint_check.start();

    var fmzReadLastPos = func() {
        var filename = getprop("/sim/fg-home") ~ "/Export/bd700-last-pos.xml";
        if (io.stat(filename) == nil) {
            var lat_deg = getprop("/position/latitude-deg");
            var lon_deg = getprop("/position/longitude-deg");
            var file = io.open(filename, "w");
            var xml  = '<?xml version="1.0" encoding="UTF-8"?>
<LastPos>
  <lat>' ~ lat_deg ~ '</lat>
  <lon>' ~ lon_deg ~ '</lon>
</LastPos>';
            io.write(file, xml);
            io.close(file);
        }
        var file_content = io.readfile(filename);
        var last_pos_begin = find("<LastPos>", file_content) + 9;
        var last_pos_end = find("</LastPos>", file_content);
        var last_pos_len = last_pos_end - last_pos_begin;
        var last_pos_content = substr(file_content, last_pos_begin, last_pos_len);
        var last_lat_begin = find("<lat>", last_pos_content) + 5;
        var last_lat_end = find("</lat>", last_pos_content);
        var last_lat_len = last_lat_end - last_lat_begin;
        var last_lat_deg = num(substr(last_pos_content, last_lat_begin, last_lat_len));
        var last_lon_begin = find("<lon>", last_pos_content) + 5;
        var last_lon_end = find("</lon>", last_pos_content);
        var last_lon_len = last_lon_end - last_lon_begin;
        var last_lon_deg = num(substr(last_pos_content, last_lon_begin, last_lon_len));
        if (last_lat_deg >= 0){
                var last_north_south = "N";
            } else {
                var last_north_south = "S";
            }
            if (last_lon_deg >=0) {
                var last_east_west = "E";
            } else {
                var last_east_west = "W";
            }
        var last_lat_deg_int = math.floor(abs(last_lat_deg));
        var last_lon_deg_int = math.floor(abs(last_lon_deg));
        var last_lat_min = (abs(last_lat_deg) - last_lat_deg_int) * 60;
        var last_lon_min = (abs(last_lon_deg) - last_lon_deg_int) * 60;
        var last_lat_deg_str = sprintf("%03d", last_lat_deg_int);
        var last_lon_deg_str = sprintf("%03d", last_lon_deg_int);
        var last_lat_min_str = sprintf("%2.1f", last_lat_min);
        var last_lon_min_str = sprintf("%2.1f", last_lon_min);
        var last_pos_str = last_north_south ~ last_lat_deg_str ~ "° " ~ last_lat_min_str ~ "  " ~ last_east_west ~ last_lon_deg_str ~ "° " ~ last_lon_min_str;
        return last_pos_str;
    }
    var last_pos_str = fmzReadLastPos();

    var pageNavIdent = func {
        if (page_title_text == "NAV IDENT") {
            eachPageInit();
            pageTitle.setText(sprintf("%s", "NAV IDENT"));
            pageTitleRight.setText(sprintf("%s", ""));
            pageTitle.setColor(255,255,255,255);
            pageTitleRight.setColor(255,255,255,255);
            totalPage.setText(sprintf("%s", "1"));
            currentPage.setText(sprintf("%s", "1"));
            totalPage.setColor(255,255,255,255);
            currentPage.setColor(255,255,255,255);
            line1Left.setText(sprintf("%s", "DATE"));
            line1Left.setColor(255,255,255,255);
            line1Left.setFontSize(36, 1);
            line2Left.setText(sprintf("%s", set_date));
            line2Left.setColor(0,174,255,255);
            line2Left.setFontSize(48, 1);
            line3Left.setText(sprintf("%s", "UTC"));
            line3Left.setColor(255,255,255,255);
            line3Left.setFontSize(36, 1);
            line4Left.setText(sprintf("%s", time_text));
            line4Left.setColor(0,174,255,255);
            line4Left.setFontSize(48, 1);
            line5Left.setText(sprintf("%s", "SW"));
            line5Left.setColor(255,255,255,255);
            line5Left.setFontSize(36, 1);
            line6Left.setText(sprintf("%s", "NZ6.125"));
            line6Left.setColor(0,255,0,255);
            line6Left.setFontSize(48, 1);
            line7Left.setText(sprintf("%s", "◀ MAINTENANCE"));
            line7Left.setColor(255,255,255,255);
            line7Left.setFontSize(36, 1);
            line1Right.setText(sprintf("%s", "ACTIVE NDB"));
            line1Right.setColor(255,255,255,255);
            line1Right.setFontSize(36, 1);
            line2Right.setText(sprintf("%s", "Data Cycle 2013.10"));
            line2Right.setColor(0,255,0,255);
            line2Right.setFontSize(48, 1);
            line3Right.setText(sprintf("%s", " "));
            line4Right.setText(sprintf("%s", "Data Cycle 2013.10 ▶"));
            line4Right.setColor(255,255,0,255);
            line4Right.setFontSize(48, 1);
            line5Right.setText(sprintf("%s", "NDB V2 . 0 4M"));
            line5Right.setColor(0,255,0,255);
            line5Right.setFontSize(36, 1);
            line6Right.setText(sprintf("%s", "WORLD 2 − 04"));
            line6Right.setColor(0,255,0,255);
            line6Right.setFontSize(48, 1);
            line7Right.setText(sprintf("%s", "POS INIT ▶"));
            line7Right.setColor(255,255,255,255);
            line7Right.setFontSize(36, 1);
            line1Mid.setText(sprintf("%s", ""));
            line2Mid.setText(sprintf("%s", ""));
            line3Mid.setText(sprintf("%s", ""));
            line4Mid.setText(sprintf("%s", ""));
            line5Mid.setText(sprintf("%s", ""));
            line6Mid.setText(sprintf("%s", ""));
            line7Mid.setText(sprintf("%s", ""));

            if (L1_press_orig == 0) {
                var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                if (press) {
                    var day = 0;
                    var month = 0;
                    var year = 0;
                    var day_checked = 0;
                    var month_checked = 0;
                    var year_checked = -1;
                    if (size(scratch_pad_text) == 7 or size(scratch_pad_text) == 9) {
                        day = num(substr(scratch_pad_text,0,2)) or 0;
                        month = numericMonth[substr(scratch_pad_text,2,3)] or 0;
                        year = num(substr(scratch_pad_text,5,2)) or -1;
                        if (size(scratch_pad_text) == 9)
                            year = num(substr(scratch_pad_text,5,4)) or -1;
                        day_checked = day >= 1 and day <= 31;
                        month_checked = month >= 1 and month <= 12;
                        year_checked = year >=0 and year <= 9999;
                    }
                    if (day_checked and month_checked and year_checked) {
                        set_date =  scratch_pad_text;
                        scratch_pad_text = "";
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                } 
            }
            L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");

            if (L2_press_orig == 0) {
                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (press) {
                    var min_checked = 0;
                    var hour_checked = 0;
                    var zulu = 0;
                    if (size(scratch_pad_text) == 5) {
                        var hour = num(substr(scratch_pad_text,0,2)) or -1;
                        var min = num(substr(scratch_pad_text,2,2)) or -1;
                        zulu = (chr(scratch_pad_text[4]) == "Z");
                        hour_checked = hour >= 0 and hour < 24;
                        min_checked = min >= 0 and min < 60;
                    }
                    if (zulu and hour_checked and min_checked) {
                        global_set_time = scratch_pad_text;
                        scratch_pad_text = "";
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                }
             }
             L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

             if (R4_press_orig == 0) {
                var press = getprop("/instrumentation/fmz/fmz-key-R4-press");
                if (press) {
                    page_title_text =  "POSITION INIT";
                }
             }
             R4_press_orig = getprop("/instrumentation/fmz/fmz-key-R4-press");
        }
    }
    fmz_setlistener(pageNavIdent);
    var pageNavIdent_timer = maketimer(1, pageNavIdent);
    pageNavIdent_timer.start();

    var pagePositionInit = func {
        if (page_title_text == "POSITION INIT") {
            eachPageInit();
            pageTitle.setText(sprintf("%s", "POSITION INIT"));
            pageTitleRight.setText(sprintf("%s", ""));
            pageTitle.setColor(255,255,255,255);
            pageTitleRight.setColor(255,255,255,255);
            totalPage.setText(sprintf("%s", "1"));
            currentPage.setText(sprintf("%s", "1"));
            totalPage.setColor(255,255,255,255);
            currentPage.setColor(255,255,255,255);
            var lat_deg = getprop("/position/latitude-deg");
            var lon_deg = getprop("/position/longitude-deg");
            if (lat_deg >= 0){
                var north_south = "N";
            } else {
                var north_south = "S";
            }
            if (lon_deg >=0) {
                var east_west = "E";
            } else {
                var east_west = "W";
            }
            if (airportinfo().lat >= 0){
                var apt_north_south = "N";
            } else {
                var apt_north_south = "S";
            }
            if (airportinfo().lon >=0) {
                var apt_east_west = "E";
            } else {
                var apt_east_west = "W";
            }
            var lat_deg_int = math.floor(abs(lat_deg));
            var lon_deg_int = math.floor(abs(lon_deg));
            var apt_lat_deg_int = math.floor(abs(airportinfo().lat));
            var apt_lon_deg_int = math.floor(abs(airportinfo().lon));           
            var lat_min = (abs(lat_deg) - lat_deg_int) * 60;
            var lon_min = (abs(lon_deg) - lon_deg_int) * 60;
            var apt_lat_min = (abs(airportinfo().lat) - apt_lat_deg_int) * 60;
            var apt_lon_min = (abs(airportinfo().lon) - apt_lon_deg_int) * 60;
            var lat_deg_str = sprintf("%03d", lat_deg_int);
            var lon_deg_str = sprintf("%03d", lon_deg_int);
            var lat_min_str = sprintf("%2.1f", lat_min);
            var lon_min_str = sprintf("%2.1f", lon_min);
            var apt_lat_deg_str = sprintf("%03d", apt_lat_deg_int);
            var apt_lon_deg_str = sprintf("%03d", apt_lon_deg_int);
            var apt_lat_min_str = sprintf("%2.1f", apt_lat_min);
            var apt_lon_min_str = sprintf("%2.1f", apt_lon_min);
            var pos_str = north_south ~ lat_deg_str ~ "° " ~ lat_min_str ~ "  " ~ east_west ~ lon_deg_str ~ "° " ~ lon_min_str;
            var apt_pos_str = apt_north_south ~ apt_lat_deg_str ~ "° " ~ apt_lat_min_str ~ "  " ~ apt_east_west ~ apt_lon_deg_str ~ "° " ~ apt_lon_min_str;
            line1Left.setText(sprintf("%s", "LAST POS"));
            line2Left.setText(sprintf("%s", last_pos_str));
            line3Left.setText(sprintf("%s", airportinfo().id));
            line4Left.setText(sprintf("%s", apt_pos_str));
            line5Left.setText(sprintf("%s", "GPS 1 POS"));
            line6Left.setText(sprintf("%s", pos_str));
            line7Left.setText(sprintf("%s", "◀ POS SENSORS"));
            line1Left.setColor(255,255,255,255);
            line2Left.setColor(0,255,0,255);
            line3Left.setColor(0,255,0,255);
            line4Left.setColor(0,255,0,255);
            line5Left.setColor(255,255,255,255);
            line6Left.setColor(0,255,0,255);
            
            if (pos_init_loaded == "LAST POS") {
                line1Right.setText(sprintf("%s", "(LOADED)     "));
            } else {
                line1Right.setText(sprintf("%s", " "));
            }
            
            line2Right.setText(sprintf("%s", "LOAD▶"));
            
            if (pos_init_loaded == "REF WPT") {
                line3Right.setText(sprintf("%s", "(LOADED)     "));
            } else {
                line3Right.setText(sprintf("%s", " "));
            }
            
            line4Right.setText(sprintf("%s", "LOAD▶"));
            
            if (pos_init_loaded == "GPS POS") {
                line5Right.setText(sprintf("%s", "(LOADED)     "));
            } else {
                line5Right.setText(sprintf("%s", " "));
            }
            
            line6Right.setText(sprintf("%s", "LOAD▶"));
            if (pos_init_loaded != "") {
                line7Right.setText(sprintf("%s", "FLT PLAN ▶"));
            } else {
                line7Right.setText(sprintf("%s", ""));
            }
            line2Right.setColor(255,255,255,255);
            line4Right.setColor(255,255,255,255);
            line5Right.setColor(255,255,255,255);
            line6Right.setColor(255,255,255,255);
            line7Right.setColor(255,255,255,255);
            line1Mid.setText(sprintf("%s", " "));
            line2Mid.setText(sprintf("%s", " "));
            line3Mid.setText(sprintf("%s", "REF WPT                     "));
            line4Mid.setText(sprintf("%s", " "));
            line5Mid.setText(sprintf("%s", " "));
            line6Mid.setText(sprintf("%s", " "));
            line7Mid.setText(sprintf("%s", " "));
            line3Mid.setColor(255,255,255,255);

            if (R1_press_orig == 0) {
                var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
                if (press) {
                    pos_init_loaded =  "LAST POS";
                }
            }
            R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");

            if (R2_press_orig == 0) {
                var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
                if (press) {
                    pos_init_loaded =  "REF WPT";
                }
            }
            R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");

            if (R3_press_orig == 0) {
                var press = getprop("/instrumentation/fmz/fmz-key-R3-press");
                if (press) {
                    pos_init_loaded =  "GPS POS";
                }
            }
            R3_press_orig = getprop("/instrumentation/fmz/fmz-key-R3-press");

            if (R4_press_orig == 0) {
                var press = getprop("/instrumentation/fmz/fmz-key-R4-press");
                if (press) {
                    if (pos_init_loaded !="") {
                        page_title_text = "ACTIVE FLT PLAN";
                    } else {
                        scratch_pad_message = "NO INITIAL POSITION LOADED";
                    }
                }
            }
            R4_press_orig = getprop("/instrumentation/fmz/fmz-key-R4-press");
        }
    }
    fmz_setlistener(pagePositionInit);
    var pagePositionInit_timer = maketimer(3, pagePositionInit);
    pagePositionInit_timer.start();

    var rte_wpt_counter = func () {
        var rte_wpt_num = 0;
        wpt_num = waypoints.size();
        for (i=0; i<wpt_num; i=i+1) {
            var proc_type = waypoints.vector[i].procedure;
            if (proc_type != "miss" and proc_type != "altn") {
                rte_wpt_num = rte_wpt_num + 1;
            }
        }
        return(rte_wpt_num);
    }

    var miss_wpt_counter = func () {
        var miss_wpt_num = 0;
        miss_wpt_vec = [];
        wpt_num = waypoints.size();
        for (i=0; i<wpt_num; i=i+1) {
            var proc_type = waypoints.vector[i].procedure;
            if (proc_type == "miss") {
                miss_wpt_num = miss_wpt_num + 1;
                append(miss_wpt_vec, i);
            }
        }
        return(miss_wpt_num);
    }


    var altn_wpt_counter = func () {
        var altn_wpt_num = 0;
        altn_wpt_vec = [];
        wpt_num = waypoints.size();
        for (i=0; i<wpt_num; i=i+1) {
            var proc_type = waypoints.vector[i].procedure;
            if (proc_type == "altn") {
                altn_wpt_num = altn_wpt_num + 1;
                append(altn_wpt_vec, i);
            }
        }
        return(altn_wpt_num);
    }

    var pageActiveFltPlan = func {
        var origin_airport = waypoints.vector[0].id;
        wpt_num = waypoints.size();
        var rte_wpt_num = rte_wpt_counter();
        var ground_spd = getprop("/velocities/groundspeed-kt");

        if (page_title_text == "ACTIVE FLT PLAN"){
            eachPageInit();
            var press = getprop("/instrumentation/fmz/fmz-key-R4-press");
            if (R4_press_orig == 0 and press == 1){
                if (is_active) {            
                    page_title_text = "PERFORMANCE INIT";
                    current_page_prop = 1;
                } else {
                    page_title_text = "FPL PLAN SELECT";
                }
            }
            R4_press_orig = getprop("/instrumentation/fmz/fmz-key-R4-press");

            var press = getprop("/instrumentation/fmz/fmz-key-L4-press");
            if (L4_press_orig == 0 and press == 1){
                if (is_active) {            
                    page_title_text = "DEPARTURE RUNWAYS";
                    current_page_prop = 1;
                } else {
                    #page_title_text = "FLIGHT PLAN LIST";
                }
            }
            L4_press_orig = getprop("/instrumentation/fmz/fmz-key-L4-press");
             
            if (rte_wpt_num >= 1) {
                total_page_rte = math.floor((rte_wpt_num + 3) / 3);
            }           
            total_page_prop = total_page_rte + total_page_miss + total_page_altn;
            if (current_page_prop >= total_page_prop){
                current_page_prop = total_page_prop;
            }
            totalPage.setText(sprintf("%s", total_page_prop));
            currentPage.setText(sprintf("%s", current_page_prop));
        
            if (current_page_prop <= total_page_rte) {
                if (dest_airport == "") {
                    pageTitle.setText(sprintf("%s", "ACTIVE FLT PLAN"));
                    pageTitleRight.setText(sprintf("%s", ""));
                    pageTitle.setColor(255,255,255,255);
                    pageTitleRight.setColor(255,255,255,255);
                    line4Right.setText(sprintf("%s", "-----"));
                    line4Right.setColor(0,174,255,255);
                } else {
                    pageTitle.setText(sprintf("%s", origin_airport ~ "-" ~ dest_airport));
                    pageTitleRight.setText(sprintf("%s", "FPL  "));
                    pageTitle.setColor(255,255,0,255);
                    pageTitleRight.setColor(255,255,255,255);               
                }
            }

            if (is_active) {
                line7Left.setText(sprintf("%s", "◀ DEPARTURE"));
                line7Left.setColor(255,255,255,255);
                line7Right.setText(sprintf("%s", "PERF INIT ▶"));
                line7Right.setColor(255,255,255,255);
            } else {
                line7Left.setText(sprintf("%s", "◀ FPL LIST"));
                line7Left.setColor(255,255,255,255);
                line7Right.setText(sprintf("%s", "FPL SEL ▶"));
                line7Right.setColor(255,255,255,255);
            }
            line7Mid.setText(sprintf("%s", ""));
            line7Mid.setColor(255,255,255,255);

            if (current_page_prop == 1 and rte_wpt_num <= 1) {

                line1Left.setText(sprintf("%s", "ORIGIN"));
                line1Left.setColor(255,255,255,255);
                line1Mid.setText(sprintf("%s", "DIST/ETE"));
                line1Mid.setColor(255,255,255,255);
                line1Right.setText(sprintf("%s", spd_rule));
                line1Right.setColor(255,255,255,255);
                line2Left.setText(sprintf("%s", origin_airport));
                line2Left.setColor(255,255,0,255);
                line2Mid.setText(sprintf("%s", ""));
                line2Mid.setColor(255,255,255,255);
                line2Right.setText(sprintf("%s", ""));
                line2Right.setColor(255,255,255,255);

                if (waypoints.vector[(waypoints.size()-1)].id == dest_airport) {
                    line3Left.setText(sprintf("%s", ""));
                    line3Left.setColor(255,255,255,255);
                    line4Left.setText(sprintf("%s", ""));
                    line4Left.setColor(0,174,255,255);
                } else {
                    line3Left.setText(sprintf("%s", "VIA TO"));
                    line3Left.setColor(255,255,255,255);
                    line4Left.setText(sprintf("%s", "-----"));
                    line4Left.setColor(0,174,255,255);
                }
                
                line3Mid.setText(sprintf("%s", ""));
                line3Mid.setColor(255,255,255,255);
                line3Right.setText(sprintf("%s", "DEST"));
                line3Right.setColor(255,255,255,255);
                line4Mid.setText(sprintf("%s", ""));
                line4Mid.setColor(255,255,255,255);
                if (dest_airport == ""){
                    line4Right.setText(sprintf("%s", "-----"));
                    line4Right.setColor(0,174,255,255);
                } else {
                    line4Right.setText(sprintf("%s", dest_airport));
                    line4Right.setColor(0,255,0,255);
                }
                line5Left.setText(sprintf("%s", ""));
                line5Mid.setText(sprintf("%s", "RECALL OR CREATE"));
                line5Mid.setColor(255,255,255,255);
                line5Right.setText(sprintf("%s", ""));
                line5Right.setColor(255,255,255,255);
                line6Left.setText(sprintf("%s", ""));
                line6Mid.setText(sprintf("%s", "FPL NAMED"));
                line6Mid.setColor(255,255,255,255);
                line6Right.setText(sprintf("%s", "-------"));
                line6Right.setColor(255,255,0,255);

                if (R2_press_orig == 0) {
                    var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
                    if (press == 1) {
                        var dest_airport_tmp = scratch_pad_text;
                        var chck_dest = findAirportsByICAO(dest_airport_tmp);
                        if (size(chck_dest) != 1) {
                            scratch_pad_message = "NOT IN DATABASE";
                            dest_airport_tmp = "";
                        } else if (chck_dest[0].id != dest_airport_tmp) {
                            scratch_pad_message = "NOT IN DATABASE";
                            dest_airport_tmp = "";
                        } else {
                            dest_airport = dest_airport_tmp;
                        }
                        scratch_pad_text = "";
                    }
                }
                R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");

                if (L2_press_orig == 0) {
                    var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                    if (press == 1) {
                        if (dest_airport != ""){
                            if (scratch_pad_text == dest_airport) {
                                var dest_apt_hash = createWaypoint.new(dest_airport, airportinfo(dest_airport).lat, airportinfo(dest_airport).lon);
                                dest_apt_hash.altitude = airportinfo(dest_airport).elevation;
                                waypoints.insert(rte_wpt_counter(),dest_apt_hash);
                            } else {
                                read_crt_wpt(scratch_pad_text, 1);
                            }
                        } else {
                            scratch_pad_message = "INVALID INPUT";
                        }
                        
                        scratch_pad_text = "";
                    }
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
            }

            if (current_page_prop == 1 and rte_wpt_num == 2){

                line1Left.setText(sprintf("%s", "ORIGIN"));
                line1Left.setColor(255,255,255,255);
                line1Mid.setText(sprintf("%s", "DIST/ETE"));
                line1Mid.setColor(255,255,255,255);
                line1Right.setText(sprintf("%s", spd_rule));
                line1Right.setColor(255,255,255,255);
                line2Left.setText(sprintf("%s", origin_airport));
                line2Left.setColor(255,255,0,255);
                line2Mid.setText(sprintf("%s", ""));
                line2Mid.setColor(255,255,255,255);
                line2Right.setText(sprintf("%s", ""));
                line2Right.setColor(255,255,255,255);
                var hdg = grtCircHdg(1);
                line3Left.setText(sprintf("%03.0f", hdg) ~ "°");
                line3Left.setColor(0,255,0,255);
                var leg_dist = grtCircDist(1);
                line3Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                line3Mid.setColor(0,255,0,255);
                line3Right.setText(sprintf("%s", ""));
                line3Right.setColor(255,255,255,255);
                line4Left.setText(sprintf("%s", waypoints.vector[1].id));
                line4Left.setColor(0,255,0,255);
                if (ground_spd < 80){
                    var ete_f = leg_dist / 440;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                } else {
                    var ete_f = leg_dist / ground_spd;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                }
                line4Mid.setText(sprintf("%s", ete_str));
                line4Mid.setColor(0,255,0,255);
                line4Right.setText(sprintf("%s", "-----"));
                line4Right.setColor(0,174,255,255);

                if (waypoints.vector[(waypoints.size()-1)].id == dest_airport) {
                    line5Left.setText(sprintf("%s", ""));
                    line5Left.setColor(255,255,255,255);
                    line6Left.setText(sprintf("%s", ""));
                    line6Left.setColor(0,174,255,255);
                } else {
                    line5Left.setText(sprintf("%s", "VIA TO"));
                    line5Left.setColor(255,255,255,255);
                    line6Left.setText(sprintf("%s", "-----"));
                    line6Left.setColor(0,174,255,255);
                }
                
                line5Right.setText(sprintf("%s", "DEST"));
                line5Right.setColor(255,255,255,255);
                line5Mid.setText(sprintf("%s", ""));
                line5Mid.setColor(255,255,255,255);
                line6Mid.setText(sprintf("%s", ""));
                line6Mid.setColor(255,255,255,255);
                line6Right.setText(sprintf("%s", dest_airport));
                line6Right.setColor(0,255,0,255);

                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1 and scratch_pad_text != "") {

                    read_crt_wpt(scratch_pad_text, 1);
                    scratch_pad_text = "";
                    
                } else if (L2_press_orig == 0 and press == 1 and scratch_pad_message == "DELETE") {
                    waypoints.pop(1);
                    scratch_pad_message = "";
                } else if (L2_press_orig == 0 and press == 1){
                    scratch_pad_message = "INVALID INPUT";
                } 
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (R3_press_orig == 0) {
                    var press = getprop("/instrumentation/fmz/fmz-key-R3-press");
                    if (press == 1) {
                        var dest_airport_tmp = scratch_pad_text;
                        var chck_dest = findAirportsByICAO(dest_airport_tmp);
                        if (size(chck_dest) != 1) {
                            scratch_pad_message = "NOT IN DATABASE";
                            dest_airport_tmp = "";
                        } else if (chck_dest[0].id != dest_airport_tmp) {
                            scratch_pad_message = "NOT IN DATABASE";
                            dest_airport_tmp = "";
                        } else {
                            dest_airport = dest_airport_tmp;
                        }
                        scratch_pad_text = "";
                    }
                }
                R3_press_orig = getprop("/instrumentation/fmz/fmz-key-R3-press");
            
                if (L3_press_orig == 0) {
                    var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                    if (press == 1) {
                        var has_dot = find(".", scratch_pad_text)!=-1;
                        var long_enough = size(scratch_pad_text) >=5;
                        if (has_dot and long_enough) {
                            var is_awy = awy_parser(scratch_pad_text);
                        } else {
                            var is_awy = 0;
                        }
                        if (rte_wpt_num == 2 and is_awy == 1) {
                            for (i=size(awy_wpt_list)-1;i>0;i=i-1) {
                                var awy_wpt = createWaypoint.new(awy_wpt_list[i], awy_wpt_list_lat[i], awy_wpt_list_lon[i]);
                                waypoints.insert(2, awy_wpt);
                            }
                            scratch_pad_text = "";
                        } else if (rte_wpt_num == 2 and is_awy ==0) {
                            if (scratch_pad_text == dest_airport) {
                                var dest_apt_hash = createWaypoint.new(dest_airport, airportinfo(dest_airport).lat, airportinfo(dest_airport).lon);
                                dest_apt_hash.altitude = airportinfo(dest_airport).elevation;
                                waypoints.insert(rte_wpt_counter(), dest_apt_hash);
                            } else {
                                read_crt_wpt(scratch_pad_text, 2);
                            }
                            scratch_pad_text = "";
                        } else {
                            scratch_pad_message = "INVALID INPUT";
                        }
                    }
                }
                L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");
            }
            if (current_page_prop == 1 and rte_wpt_num >= 3){

                line1Left.setText(sprintf("%s", "ORIGIN"));
                line1Left.setColor(255,255,255,255);
                line1Mid.setText(sprintf("%s", "DIST/ETE"));
                line1Mid.setColor(255,255,255,255);
                line1Right.setText(sprintf("%s", spd_rule));
                line1Right.setColor(255,255,255,255);
                line2Left.setText(sprintf("%s", origin_airport));
                line2Left.setColor(255,255,0,255);
                line2Mid.setText(sprintf("%s", ""));
                line2Mid.setColor(255,255,255,255);
                line2Right.setText(sprintf("%s", ""));
                line2Right.setColor(255,255,255,255);
                var hdg = grtCircHdg(1);                
                line3Left.setText(sprintf("%03.0f", hdg) ~ "°");
                line3Left.setColor(0,255,0,255);
                var leg_dist = grtCircDist(1);
                line3Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                line3Mid.setColor(0,255,0,255);
                line3Right.setText(sprintf("%s", ""));
                line3Right.setColor(255,255,255,255);
                line4Left.setText(sprintf("%s", waypoints.vector[1].id));
                line4Left.setColor(0,255,0,255);
                if (ground_spd < 80){
                    var ete_f = leg_dist / 440;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                } else {
                    var ete_f = leg_dist / ground_spd;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                }
                line4Mid.setText(sprintf("%s", ete_str));
                line4Mid.setColor(0,255,0,255);
                line4Right.setText(sprintf("%s", ""));
                line4Right.setColor(255,255,255,255); 
                var hdg = grtCircHdg(2);
                line5Left.setText(sprintf("%03.0f", hdg) ~ "°");
                line5Left.setColor(0,255,0,255);
                var leg_dist = grtCircDist(2);
                line5Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                line5Mid.setColor(0,255,0,255);
                line5Right.setText(sprintf("%s", ""));
                line5Right.setColor(255,255,255,255);
                line6Left.setText(sprintf("%s", waypoints.vector[2].id));
                line6Left.setColor(0,255,0,255);
                if (ground_spd < 80){
                    var ete_f = leg_dist / 440;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                } else {
                    var ete_f = leg_dist / ground_spd;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                }
                line6Mid.setText(sprintf("%s", ete_str));
                line6Mid.setColor(0,255,0,255);
                line6Right.setText(sprintf("%s", "-----"));
                line6Right.setColor(0,174,255,255);

                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1 and scratch_pad_text != "") {

                    read_crt_wpt(scratch_pad_text, 1);
                    scratch_pad_text = "";
                    
                } else if (L2_press_orig == 0 and press == 1 and scratch_pad_message == "DELETE") {
                    waypoints.pop(1);
                    scratch_pad_message = "";
                } else if (L2_press_orig == 0 and press == 1) {
                    scratch_pad_message = "INVALID INPUT";
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
                
                var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                if (L3_press_orig == 0 and press == 1 and scratch_pad_text != "") {
                    
                    read_crt_wpt(scratch_pad_text, 2);
                    scratch_pad_text = "";
                    
                } else if (L3_press_orig == 0 and press == 1 and scratch_pad_message == "DELETE") {
                    waypoints.pop(2);
                    scratch_pad_message = "";
                }
                L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");
            }

            if (current_page_prop >= 2 and current_page_prop == total_page_rte and math.fmod(rte_wpt_num,3) == 0){

                if (waypoints.vector[(waypoints.size()-1)].id == dest_airport) {
                    line1Left.setText(sprintf("%s", ""));
                    line1Left.setColor(255,255,255,255);
                    line2Left.setText(sprintf("%s", ""));
                    line2Left.setColor(0,174,255,255);
                } else {
                    line1Left.setText(sprintf("%s", "VIA TO"));
                    line1Left.setColor(255,255,255,255);
                    line2Left.setText(sprintf("%s", "-----"));
                    line2Left.setColor(0,174,255,255);
                }
                line1Mid.setText(sprintf("%s", ""));
                line1Mid.setColor(255,255,255,255);
                line1Right.setText(sprintf("%s", "DEST"));
                line1Right.setColor(255,255,255,255);
                line2Mid.setText(sprintf("%s", ""));
                line2Mid.setColor(255,255,255,255);
                line2Right.setText(sprintf("%s", dest_airport));
                line2Right.setColor(0,255,0,255);
                line3Left.setText(sprintf("%s", ""));
                line3Left.setColor(255,255,255,255);
                line3Mid.setText(sprintf("%s", ""));
                line3Mid.setColor(255,255,255,255);
                line3Right.setText(sprintf("%s", ""));
                line3Right.setColor(255,255,255,255);
                line4Left.setText(sprintf("%s", ""));
                line4Left.setColor(255,255,255,255);
                line4Mid.setText(sprintf("%s", ""));
                line4Mid.setColor(255,255,255,255);
                line4Right.setText(sprintf("%s", ""));
                line4Right.setColor(255,255,255,255);
                line5Left.setText(sprintf("%s", ""));
                line5Left.setColor(255,255,255,255);
                line5Mid.setText(sprintf("%s", ""));
                line5Mid.setColor(255,255,255,255);
                line5Right.setText(sprintf("%s", ""));
                line5Right.setColor(255,255,255,255);
                line6Left.setText(sprintf("%s", ""));
                line6Left.setColor(255,255,255,255);
                line6Mid.setText(sprintf("%s", ""));
                line6Mid.setColor(255,255,255,255);
                line6Right.setText(sprintf("%s", ""));
                line6Right.setColor(255,255,255,255);

                if (L1_press_orig == 0) {
                    var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                    if (press == 1) {
                        if (dest_airport != ""){
                            var has_dot = find(".", scratch_pad_text)!=-1;
                            var long_enough = size(scratch_pad_text) >=5;
                            if (has_dot and long_enough) {
                                var is_awy = awy_parser(scratch_pad_text);
                            } else {
                                var is_awy = 0;
                            }
                            if (is_awy == 1) {
                                for (i=size(awy_wpt_list)-1;i>0;i=i-1) {
                                    var awy_wpt = createWaypoint.new(awy_wpt_list[i], awy_wpt_list_lat[i], awy_wpt_list_lon[i]);
                                    waypoints.insert((current_page_prop*3-3), awy_wpt);
                                }
                                scratch_pad_text = "";
                            } else {
                                if (scratch_pad_text == dest_airport) {
                                    var dest_apt_hash = createWaypoint.new(dest_airport,airportinfo(dest_airport).lat, airportinfo(dest_airport).lon);
                                    dest_apt_hash.altitude = airportinfo(dest_airport).elevation;
                                    waypoints.insert(rte_wpt_counter(), dest_apt_hash);
                                } else {
                                    read_crt_wpt(scratch_pad_text, (current_page_prop*3-3));
                                }
                            }
                        } else {
                            scratch_pad_message = "INVALID INPUT";
                        }
                        
                        scratch_pad_text = "";
                    }
                }
                L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");
            }

            if (current_page_prop >= 2 and current_page_prop == total_page_rte and math.fmod(rte_wpt_num,3) == 1){
 
                var hdg = grtCircHdg(current_page_prop*3-3);
                line1Left.setText(sprintf("%03.0f", hdg) ~ "°");
                line1Left.setColor(0,255,0,255);
                var leg_dist = grtCircDist(current_page_prop*3-3);
                line1Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                line1Mid.setColor(0,255,0,255);
                line1Right.setText(sprintf("%s", ""));
                line1Right.setColor(255,255,255,255);
                var wpt_id = waypoints.vector[(current_page_prop*3-3)].id;
                line2Left.setText(sprintf("%s", wpt_id));
                line2Left.setColor(0,255,0,255);
                if (ground_spd < 80){
                    var ete_f = leg_dist / 440;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                } else {
                    var ete_f = leg_dist / ground_spd;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                }
                line2Mid.setText(sprintf("%s", ete_str));
                line2Mid.setColor(0,255,0,255);
                line2Right.setText(sprintf("%s",""));
                line2Right.setColor(255,255,255,255);

                if (waypoints.vector[(waypoints.size()-1)].id == dest_airport) {
                    line3Left.setText(sprintf("%s", ""));
                    line3Left.setColor(255,255,255,255);
                    line4Left.setText(sprintf("%s", ""));
                    line4Left.setColor(0,174,255,255);
                } else {
                    line3Left.setText(sprintf("%s", "VIA TO"));
                    line3Left.setColor(255,255,255,255);
                    line4Left.setText(sprintf("%s", "-----"));
                    line4Left.setColor(0,174,255,255);
                }
                line3Mid.setText(sprintf("%s", ""));
                line3Mid.setColor(255,255,255,255);
                line3Right.setText(sprintf("%s", "DEST"));
                line3Right.setColor(255,255,255,255);
                line4Mid.setText(sprintf("%s", ""));
                line4Mid.setColor(255,255,255,255);
                line4Right.setText(sprintf("%s", dest_airport));
                line4Right.setColor(0,255,0,255);
                line5Left.setText(sprintf("%s", ""));
                line5Left.setColor(255,255,255,255);
                line5Mid.setText(sprintf("%s", ""));
                line5Mid.setColor(255,255,255,255);
                line5Right.setText(sprintf("%s", ""));
                line5Right.setColor(255,255,255,255);
                line6Left.setText(sprintf("%s", ""));
                line6Left.setColor(255,255,255,255);
                line6Mid.setText(sprintf("%s", ""));
                line6Mid.setColor(255,255,255,255);
                line6Right.setText(sprintf("%s", ""));
                line6Right.setColor(255,255,255,255);
           
                var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                if (L1_press_orig == 0 and press == 1 and scratch_pad_text != ""){
                    read_crt_wpt(scratch_pad_text, (current_page_prop*3-3));
                    scratch_pad_text = "";
                } else if (L1_press_orig == 0 and press == 1 and scratch_pad_message == "DELETE") {
                    waypoints.pop((current_page_prop*3-3));
                    scratch_pad_message = "";
                } else if (L1_press_orig == 0 and press == 1) {
                    scratch_pad_message = "INVALID INPUT";
                }
                L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");
                if (L2_press_orig == 0) {
                    var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                    if (press == 1) {
                        if (dest_airport != ""){
                            var has_dot = find(".", scratch_pad_text)!=-1;
                            var long_enough = size(scratch_pad_text) >=5;
                            if (has_dot and long_enough) {
                                var is_awy = awy_parser(scratch_pad_text);
                            } else {
                                var is_awy = 0;
                            }
                            if (is_awy == 1) {
                                for (i=size(awy_wpt_list)-1;i>0;i=i-1) {
                                    var awy_wpt = createWaypoint.new(awy_wpt_list[i], awy_wpt_list_lat[i], awy_wpt_list_lon[i]);
                                    waypoints.insert((current_page_prop*3-2), awy_wpt);
                                }
                                scratch_pad_text = "";
                            } else {
                                if (scratch_pad_text == dest_airport) {
                                    var dest_apt_hash = createWaypoint.new(dest_airport, airportinfo(dest_airport).lat, airportinfo(dest_airport).lon);
                                    dest_apt_hash.altitude = airportinfo(dest_airport).elevation;
                                    waypoints.insert(rte_wpt_counter(), dest_apt_hash);
                                } else {
                                    read_crt_wpt(scratch_pad_text, (current_page_prop*3-2));
                                }
                                scratch_pad_text = "";
                            }
                        } else {
                            scratch_pad_message = "INVALID INPUT";
                        }
                        
                        scratch_pad_text = "";
                    }
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
            }

            if (current_page_prop >= 2 and current_page_prop == total_page_rte and math.fmod(rte_wpt_num,3) == 2){

                var hdg = grtCircHdg(current_page_prop*3-3);
                line1Left.setText(sprintf("%03.0f", hdg) ~ "°");
                line1Left.setColor(0,255,0,255);
                var leg_dist = grtCircDist(current_page_prop*3-3);
                line1Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                line1Mid.setColor(0,255,0,255);
                line1Right.setText(sprintf("%s", ""));
                line1Right.setColor(255,255,255,255);
                var wpt_id = waypoints.vector[(current_page_prop*3-3)].id;
                line2Left.setText(sprintf("%s", wpt_id));
                line2Left.setColor(0,255,0,255);
                if (ground_spd < 80){
                    var ete_f = leg_dist / 440;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                } else {
                    var ete_f = leg_dist / ground_spd;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                }
                line2Mid.setText(sprintf("%s", ete_str));
                line2Mid.setColor(0,255,0,255);
                line2Right.setText(sprintf("%s",""));
                line2Right.setColor(255,255,255,255);
                var hdg = grtCircHdg(current_page_prop*3-2);
                line3Left.setText(sprintf("%03.0f", hdg) ~ "°");
                line3Left.setColor(0,255,0,255);
                var leg_dist = grtCircDist(current_page_prop*3-2);
                line3Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                line3Mid.setColor(0,255,0,255);
                line3Right.setText(sprintf("%s", ""));
                line3Right.setColor(255,255,255,255);
                var wpt_id = waypoints.vector[(current_page_prop*3-2)].id;
                line4Left.setText(sprintf("%s", wpt_id));
                line4Left.setColor(0,255,0,255);
                if (ground_spd < 80){
                    var ete_f = leg_dist / 440;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                } else {
                    var ete_f = leg_dist / ground_spd;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                }
                line4Mid.setText(sprintf("%s", ete_str));
                line4Mid.setColor(0,255,0,255);
                line4Right.setText(sprintf("%s", ""));
                line4Right.setColor(255,255,255,255);

                if (waypoints.vector[(waypoints.size()-1)].id == dest_airport) {
                    line5Left.setText(sprintf("%s", ""));
                    line5Left.setColor(255,255,255,255);
                    line6Left.setText(sprintf("%s", ""));
                    line6Left.setColor(0,174,255,255);
                } else {
                    line5Left.setText(sprintf("%s", "VIA TO"));
                    line5Left.setColor(255,255,255,255);
                    line6Left.setText(sprintf("%s", "-----"));
                    line6Left.setColor(0,174,255,255);
                }
                line5Mid.setText(sprintf("%s", ""));
                line5Mid.setColor(255,255,255,255);
                line5Right.setText(sprintf("%s", "DEST"));
                line5Right.setColor(255,255,255,255);
                line6Mid.setText(sprintf("%s", ""));
                line6Mid.setColor(255,255,255,255);
                line6Right.setText(sprintf("%s", dest_airport));
                line6Right.setColor(0,255,0,255);

                var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                if (L1_press_orig == 0 and press == 1 and scratch_pad_text != ""){
                    read_crt_wpt(scratch_pad_text, (current_page_prop*3-3));
                    scratch_pad_text = "";
                } else if (L1_press_orig == 0 and press == 1 and scratch_pad_message == "DELETE") {
                    waypoints.pop(current_page_prop*3-3);
                    scratch_pad_message = "";
                } else if (L1_press_orig == 0 and press == 1) {
                    scratch_pad_message = "INVALID INPUT";
                }
                L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");
                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1 and scratch_pad_text != "") {
                    read_crt_wpt(scratch_pad_text, (current_page_prop*3-2));
                    scratch_pad_text = "";
                    
                } else if (L2_press_orig == 0 and press == 1 and scratch_pad_message == "DELETE") {
                    waypoints.pop(current_page_prop*3-2);
                    scratch_pad_message = "";
                } else if (L2_press_orig == 0 and press == 1) {
                    scratch_pad_message = "INVALID INPUT";
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L3_press_orig == 0) {
                    var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                    if (press == 1) {
                        if (dest_airport != ""){
                            var has_dot = find(".", scratch_pad_text)!=-1;
                            var long_enough = size(scratch_pad_text) >=5;
                            if (has_dot and long_enough) {
                                var is_awy = awy_parser(scratch_pad_text);
                            } else {
                                var is_awy = 0;
                            }
                            if (is_awy == 1) {
                                for (i=size(awy_wpt_list)-1;i>0;i=i-1) {
                                    var awy_wpt = createWaypoint.new(awy_wpt_list[i], awy_wpt_list_lat[i], awy_wpt_list_lon[i]);
                                    waypoints.insert((current_page_prop*3-1), awy_wpt);
                                }
                                scratch_pad_text = "";
                            } else {
                                if (scratch_pad_text == dest_airport) {
                                var dest_apt_hash = createWaypoint.new(dest_airport, airportinfo(dest_airport).lat, airportinfo(dest_airport).lon);
                                dest_apt_hash.altitude = airportinfo(dest_airport).elevation;
                                waypoints.insert(rte_wpt_counter(), dest_apt_hash);
                                } else {
                                    read_crt_wpt(scratch_pad_text, (current_page_prop*3-1));
                                }
                                scratch_pad_text = "";
                            }
                        } else {
                            scratch_pad_message = "INVALID INPUT";
                        }
                        
                        scratch_pad_text = "";
                    }
                }
                L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");
            }

            if (current_page_prop >= 2 and current_page_prop < total_page_rte){

                var hdg = grtCircHdg(current_page_prop*3-3);               
                line1Left.setText(sprintf("%03.0f", hdg) ~ "°");
                line1Left.setColor(0,255,0,255);
                var leg_dist = grtCircDist(current_page_prop*3-3);
                line1Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                line1Mid.setColor(0,255,0,255);
                line1Right.setText(sprintf("%s", ""));
                line1Right.setColor(255,255,255,255);
                var wpt_id = waypoints.vector[(current_page_prop*3-3)].id;
                line2Left.setText(sprintf("%s", wpt_id));
                line2Left.setColor(0,255,0,255);
                if (ground_spd < 80){
                    var ete_f = leg_dist / 440;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                } else {
                    var ete_f = leg_dist / ground_spd;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                }
                line2Mid.setText(sprintf("%s", ete_str));
                line2Mid.setColor(0,255,0,255);
                line2Right.setText(sprintf("%s",""));
                line2Right.setColor(255,255,255,255);
                var hdg = grtCircHdg(current_page_prop*3-2);
                line3Left.setText(sprintf("%03.0f", hdg) ~ "°");
                line3Left.setColor(0,255,0,255);
                var leg_dist = grtCircDist(current_page_prop*3-2);
                line3Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                line3Mid.setColor(0,255,0,255);
                line3Right.setText(sprintf("%s", ""));
                line3Right.setColor(255,255,255,255);
                var wpt_id = waypoints.vector[(current_page_prop*3-2)].id;
                line4Left.setText(sprintf("%s", wpt_id));
                line4Left.setColor(0,255,0,255);
                if (ground_spd < 80){
                    var ete_f = leg_dist / 440;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                } else {
                    var ete_f = leg_dist / ground_spd;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                }
                line4Mid.setText(sprintf("%s", ete_str));
                line4Mid.setColor(0,255,0,255);
                line4Right.setText(sprintf("%s", ""));
                line4Right.setColor(255,255,255,255);
                var hdg = grtCircHdg(current_page_prop*3-1);
                line5Left.setText(sprintf("%03.0f", hdg) ~ "°");
                line5Left.setColor(0,255,0,255);
                var leg_dist = grtCircDist(current_page_prop*3-1);
                line5Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                line5Mid.setColor(0,255,0,255);
                line5Right.setText(sprintf("%s", ""));
                line5Right.setColor(255,255,255,255);
                var wpt_id = waypoints.vector[(current_page_prop*3-1)].id;
                line6Left.setText(sprintf("%s", wpt_id));
                line6Left.setColor(0,255,0,255);
                if (ground_spd < 80){
                    var ete_f = leg_dist / 440;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                } else {
                    var ete_f = leg_dist / ground_spd;
                    var ete_h = sprintf("%02d", ete_f);
                    var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                    var ete_str = ete_h ~ " + " ~ ete_m;
                }
                line6Mid.setText(sprintf("%s", ete_str));
                line6Mid.setColor(0,255,0,255);
                line6Right.setText(sprintf("%s", "-----"));
                line6Right.setColor(0,174,255,255);

                var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                if (L1_press_orig == 0 and press == 1 and scratch_pad_text != ""){
                    read_crt_wpt(scratch_pad_text, (current_page_prop*3-3));
                    scratch_pad_text = "";
                } else if (L1_press_orig == 0 and press == 1 and scratch_pad_message == "DELETE") {
                    waypoints.pop(current_page_prop*3-3);
                    scratch_pad_message = "";
                } else if (L1_press_orig == 0 and press == 1) {
                    scratch_pad_message = "INVALID INPUT";
                }
                L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");
                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1 and scratch_pad_text != "") {
                    read_crt_wpt(scratch_pad_text, (current_page_prop*3-2));
                    scratch_pad_text = "";
                    
                } else if (L2_press_orig == 0 and press == 1 and scratch_pad_message == "DELETE") {
                    waypoints.pop(current_page_prop*3-2);
                    scratch_pad_message = "";
                } else if (L2_press_orig == 0 and press == 1) {
                    scratch_pad_message = "INVALID INPUT";
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
                var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                if (L3_press_orig == 0 and press == 1 and scratch_pad_text != "") {
                    read_crt_wpt(scratch_pad_text, (current_page_prop*3-1));
                    scratch_pad_text = "";
                    
                } else if (L3_press_orig == 0 and press == 1 and scratch_pad_message == "DELETE") {
                    waypoints.pop(current_page_prop*3-1);
                    scratch_pad_message = "";
                } else if (L3_press_orig == 0 and press == 1) {
                    scratch_pad_message = "INVALID INPUT";
                }
                L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");
            }

            if (current_page_prop > total_page_rte and current_page_prop <= (total_page_rte + total_page_miss)) {
                pageTitle.setText("MISSED APPROACH");
                pageTitle.setColor(255,255,255,255);
                pageTitleRight.setText("");
                line1Left.setText("");
                line1Mid.setText("");
                line1Right.setText("");
                line2Left.setText("");
                line2Mid.setText("");
                line2Right.setText("");
                line3Left.setText("");
                line3Mid.setText("");
                line3Right.setText("");
                line4Left.setText("");
                line4Mid.setText("");
                line4Right.setText("");
                line5Left.setText("");
                line5Mid.setText("");
                line5Right.setText("");
                line6Left.setText("");
                line6Mid.setText("");
                line6Right.setText("");
                line7Left.setText("");
                line7Mid.setText("");
                line7Right.setText("");
            }

            if (current_page_prop > (total_page_rte + total_page_miss) and current_page_prop <= total_page_prop) {

                pageTitle.setText("ALTERNATE FPL");
                pageTitle.setColor(255,255,255,255);
                pageTitleRight.setText("");
                var altn_wpt_num = altn_wpt_counter();
                var non_altn_num = rte_wpt_counter() + miss_wpt_counter();
                total_page_altn = math.floor((altn_wpt_num - 2) / 3) + 2;
                var current_page_altn = current_page_prop - total_page_rte - total_page_miss;

                if (altn_wpt_num == 0) {
                    line1Left.setText("");
                    line1Mid.setText("");
                    line1Right.setText("");
                    line2Mid.setText("");
                    line2Right.setText("");
                    line3Mid.setText("");
                    line4Mid.setText("");
                    if (altn_airport == "") {
                        line2Left.setText("");
                        line3Left.setText("");
                        line3Right.setText("ALTERNATE    ");
                        line3Right.setColor(255,255,255,255);
                        line4Left.setText("");
                        line4Right.setText("-----");
                        line4Right.setColor(0,174,255,255);
                    } else {
                        line2Left.setText(dest_airport);
                        line2Left.setColor(0,255,0,255);
                        line3Left.setText("VIA.TO");
                        line3Left.setColor(255,255,255,255);
                        line3Right.setText("ALTN       ");
                        line3Right.setColor(255,255,255,255);
                        line4Left.setText("-----");
                        line4Left.setColor(0,174,255,255);
                        line4Right.setText(altn_airport);
                        line4Right.setColor(0,255,0,255);
                    }
                    line5Left.setText("");
                    line5Mid.setText("");
                    line5Right.setText("");
                    line6Left.setText("");
                    line6Mid.setText("");
                    line6Right.setText("");

                    var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
                    if (R2_press_orig == 0 and press == 1) {
                        var altn_airport_tmp = scratch_pad_text;
                        var chck_altn = findAirportsByICAO(altn_airport_tmp);
                        if (size(chck_altn) != 1) {
                            scratch_pad_message = "NOT IN DATABASE";
                            altn_airport_tmp = "";
                        } else if (chck_altn[0].id != altn_airport_tmp) {
                            scratch_pad_message = "NOT IN DATABASE";
                            altn_airport_tmp = "";
                        } else {
                            altn_airport = altn_airport_tmp;
                        }
                        scratch_pad_text = "";
                    }
                    R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");

                    var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                    if (L2_press_orig == 0 and press == 1) {
                        var has_dot = find(".", scratch_pad_text)!=-1;
                        var long_enough = size(scratch_pad_text) >=5;
                        if (has_dot and long_enough) {
                            var is_awy = awy_parser(scratch_pad_text);
                        } else {
                            var is_awy = 0;
                        }
                        if (is_awy == 1) {
                            var insert_point = non_altn_num + 0;
                            for (i=size(awy_wpt_list)-1;i>0;i=i-1) {
                                var awy_wp = createWaypoint.new(awy_wpt_list[i], awy_wpt_list_lat[i], awy_wpt_list_lon[i]);
                                awy_ap.procedure = "altn";
                                waypoints.insert(insert_point, awy_wp);
                            }
                            scratch_pad_text = "";
                        } else {
                            if (scratch_pad_text == altn_airport) {
                                var altn_apt_hash = createWaypoint.new(altn_airport, airportinfo(altn_airport).lat, airportinfo(altn_airport).lon);
                                altn_apt_hash.altitude = airportinfo(altn_airport).elevation;
                                altn_apt_hash.procedure = "altn";
                                waypoints.append(altn_apt_hash);
                            } else {
                                var old_wpt_num = waypoints.size();
                                var insert_point = non_altn_num + 0;
                                read_crt_wpt(scratch_pad_text, insert_point);
                                var new_wpt_num = waypoints.size();
                                if (new_wpt_num > old_wpt_num) {
                                    waypoints.vector[wpt_sel_isr].procedure = "altn";
                                }
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
                }

                if (altn_wpt_num == 1) {
                    line1Left.setText("");
                    line1Mid.setText("");
                    line1Right.setText("");
                    line2Left.setText(dest_airport);
                    line2Left.setColor(0,255,0,255);
                    line2Mid.setText("");
                    line2Right.setText("");
                    var hdg = grtCircHdg(altn_wpt_vec[0]);
                    line3Left.setText(sprintf("%03d", hdg) ~ "°");
                    line3Left.setColor(0,255,0,255);
                    var leg_dist = grtCircDist(altn_wpt_vec[0]);
                    line3Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                    line3Mid.setColor(0,255,0,255);
                    line3Right.setText("");
                    var wpt_id = waypoints.vector[altn_wpt_vec[0]].id;
                    line4Left.setText(wpt_id);
                    line4Left.setColor(0,255,0,255);
                    if (ground_spd < 80){
                        var ete_f = leg_dist / 440;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    } else {
                        var ete_f = leg_dist / ground_spd;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    }
                    line4Mid.setText(ete_str);
                    line4Mid.setColor(0,255,0,255);
                    line4Right.setText("");
                    line5Left.setText("VIA.TO");
                    line5Left.setColor(255,255,255,255);
                    line5Mid.setText("");
                    line5Right.setText("ALTN");
                    line5Right.setColor(255,255,255,255);
                    line6Left.setText("------");
                    line6Left.setColor(0,174,255,255);
                    line6Mid.setText("");
                    line6Right.setText(altn_airport);
                    line6Right.setColor(0,255,0,255);

                    var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                    if (L2_press_orig == 0 and press == 1) {
                        if (scratch_pad_message == "DELETE"){
                            var pop_point = non_altn_num + 0;
                            waypoints.pop(pop_point);
                            scratch_pad_message = "";
                        } else {
                            var old_wpt_num = waypoints.size();
                            var insert_point = non_altn_num + 0;
                            read_crt_wpt(scratch_pad_text, insert_point);
                            var new_wpt_num = waypoints.size();
                            if (new_wpt_num > old_wpt_num) {
                                waypoints.vector[wpt_sel_isr].procedure = "altn";
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
                    
                    var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                    if (L3_press_orig == 0 and press == 1) {
                        var old_wpt_num = waypoints.size();
                        var has_dot = find(".", scratch_pad_text)!=-1;
                        var long_enough = size(scratch_pad_text) >=5;
                        if (has_dot and long_enough) {
                            var is_awy = awy_parser(scratch_pad_text);
                        } else {
                            var is_awy = 0;
                        }
                        if (is_awy == 1) {
                            var insert_point = non_altn_num + 1;
                            for (i=size(awy_wpt_list)-1;i>0;i=i-1) {
                                var awy_wp = createWaypoint.new(awy_wpt_list[i], awy_wpt_list_lat[i], awy_wpt_list_lon[i]);
                                awy_wp.procedure = "altn";
                                waypoints.insert(insert_point, awy_wp);
                            }
                            scratch_pad_text = "";
                        } else {
                            if (scratch_pad_text == altn_airport) {
                                var altn_apt_hash = createWaypoint.new(altn_airport, airportinfo(altn_airport).lat, airportinfo(altn_airport).lon);
                                altn_apt_hash.altitude = airportinfo(altn_airport).elevation;
                                altn_apt_hash.procedure = "altn";
                                waypoints.append(altn_apt_hash);
                            } else {
                                var old_wpt_num = waypoints.size();
                                var insert_point = non_altn_num + 1;
                                read_crt_wpt(scratch_pad_text, insert_point);
                                var new_wpt_num = waypoints.size();
                                if (new_wpt_num > old_wpt_num) {
                                    waypoints.vector[wpt_sel_isr].procedure = "altn";
                                }
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");
                }

                if (altn_wpt_num >= 2 and current_page_prop == (total_page_rte + total_page_miss + 1)) {
                    line1Left.setText("");
                    line1Mid.setText("");
                    line1Right.setText("");
                    line2Left.setText(dest_airport);
                    line2Left.setColor(0,255,0,255);
                    line2Mid.setText("");
                    line2Right.setText("");
                    var hdg = grtCircHdg(altn_wpt_vec[0]);
                    line3Left.setText(sprintf("%03d", hdg) ~ "°");
                    line3Left.setColor(0,255,0,255);
                    var leg_dist = grtCircDist(altn_wpt_vec[0]);
                    line3Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                    line3Mid.setColor(0,255,0,255);
                    line3Right.setText("");
                    var wpt_id = waypoints.vector[altn_wpt_vec[0]].id;
                    line4Left.setText(wpt_id);
                    line4Left.setColor(0,255,0,255);
                    if (ground_spd < 80){
                        var ete_f = leg_dist / 440;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    } else {
                        var ete_f = leg_dist / ground_spd;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    }
                    line4Mid.setText(ete_str);
                    line4Mid.setColor(0,255,0,255);
                    line4Right.setText("");
                    var hdg = grtCircHdg(altn_wpt_vec[1]);
                    line5Left.setText(sprintf("%03d", hdg) ~ "°");
                    line5Left.setColor(0,255,0,255);
                    var leg_dist = grtCircDist(altn_wpt_vec[1]);
                    line5Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                    line5Mid.setColor(0,255,0,255);
                    line5Right.setText("");
                    var wpt_id = waypoints.vector[altn_wpt_vec[1]].id;
                    line6Left.setText(wpt_id);
                    line6Left.setColor(0,255,0,255);
                    if (ground_spd < 80){
                        var ete_f = leg_dist / 440;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    } else {
                        var ete_f = leg_dist / ground_spd;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    }
                    line6Mid.setText(ete_str);
                    line6Right.setText("");
                    var press = getprop("/instrumentation/fmz/fmz-key-L2-press");

                    if (L2_press_orig == 0 and press == 1) {
                        if (scratch_pad_message == "DELETE"){
                            var pop_point = non_altn_num + 0;
                            waypoints.pop(pop_point);
                            scratch_pad_message = "";
                        } else {
                            var old_wpt_num = waypoints.size();
                            var insert_point = non_altn_num + 0;
                            read_crt_wpt(scratch_pad_text, insert_point);
                            var new_wpt_num = waypoints.size();
                            if (new_wpt_num > old_wpt_num) {
                                waypoints.vector[wpt_sel_isr].procedure = "altn";
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
                    var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                    if (L3_press_orig == 0 and press == 1) {
                        if (scratch_pad_message == "DELETE"){
                            var pop_point = non_altn_num + 1;
                            waypoints.pop(pop_point);
                            scratch_pad_message = "";
                        } else {
                            var old_wpt_num = waypoints.size();
                            var insert_point = non_altn_num + 1;
                            read_crt_wpt(scratch_pad_text, insert_point);
                            var new_wpt_num = waypoints.size();
                            if (new_wpt_num > old_wpt_num) {
                                waypoints.vector[wpt_sel_isr].procedure = "altn";
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");
                }

                if (altn_wpt_num >= 5 and current_page_prop == (total_page_rte + total_page_miss + 1)) {
                    line1Left.setText("");
                    line1Mid.setText("");
                    line1Right.setText("");
                    line2Left.setText(dest_airport);
                    line2Left.setColor(0,255,0,255);
                    line2Mid.setText("");
                    line2Right.setText("");
                    var hdg = grtCircHdg(altn_wpt_vec[current_page_altn * 3 - 3]);
                    line3Left.setText(sprintf("%03d", hdg) ~ "°");
                    line3Left.setColor(0,255,0,255);
                    var leg_dist = grtCircDist(altn_wpt_vec[current_page_altn * 3 - 3]);
                    line3Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                    line3Mid.setColor(0,255,0,255);
                    line3Right.setText("");
                    var wpt_id = waypoints.vector[altn_wpt_vec[current_page_altn * 3 - 3]].id;
                    line4Left.setText(wpt_id);
                    line4Left.setColor(0,255,0,255);
                    if (ground_spd < 80){
                        var ete_f = leg_dist / 440;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    } else {
                        var ete_f = leg_dist / ground_spd;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    }
                    line4Mid.setText(ete_str);
                    line4Mid.setColor(0,255,0,255);
                    line4Right.setText("");
                    var hdg = grtCircHdg(altn_wpt_vec[current_page_altn * 3 - 2]);
                    line5Left.setText(sprintf("%03d", hdg) ~ "°");
                    line5Left.setColor(0,255,0,255);
                    var leg_dist = grtCircDist(altn_wpt_vec[current_page_altn * 3 - 2]);
                    line5Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                    line5Mid.setColor(0,255,0,255);
                    line5Right.setText("");
                    var wpt_id = waypoints.vector[altn_wpt_vec[current_page_altn * 3 - 2]].id;
                    line6Left.setText(wpt_id);
                    line6Left.setColor(0,255,0,255);
                    if (ground_spd < 80){
                        var ete_f = leg_dist / 440;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    } else {
                        var ete_f = leg_dist / ground_spd;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    }
                    line6Mid.setText(ete_str);
                    line6Mid.setColor(0,255,0,255);
                    line6Right.setText("");
                    
                    var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                    if (L2_press_orig == 0 and press == 1) {
                        if (scratch_pad_message == "DELETE"){
                            var pop_point = non_altn_num + (current_page_altn * 3 - 3);
                            waypoints.pop(pop_point);
                            scratch_pad_message = "";
                        } else {
                            var old_wpt_num = waypoints.size();
                            var insert_point = non_altn_num + (current_page_altn * 3 - 3);
                            read_crt_wpt(scratch_pad_text, insert_point);
                            var new_wpt_num = waypoints.size();
                            if (new_wpt_num > old_wpt_num) {
                                waypoints.vector[wpt_sel_isr].procedure = "altn";
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
                    
                    var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                    if (L3_press_orig == 0 and press == 1) {
                        if (scratch_pad_message == "DELETE"){
                            var pop_point = non_altn_num + (current_page_altn * 3 - 2);
                            waypoints.pop(pop_point);
                            scratch_pad_message = "";
                        } else {
                            var old_wpt_num = waypoints.size();
                            var insert_point = non_altn_num + (current_page_altn * 3 - 2);
                            read_crt_wpt(scratch_pad_text, insert_point);
                            var new_wpt_num = waypoints.size();
                            if (new_wpt_num > old_wpt_num) {
                                waypoints.vector[wpt_sel_isr].procedure = "altn";
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");
                } else if (altn_wpt_num >= 5 and current_page_prop < total_page_prop) {
                    var hdg = grtCircHdg(altn_wpt_vec[current_page_altn * 3 - 4]);
                    line1Left.setText(sprintf("%03d", hdg) ~ "°");
                    line1Left.setColor(0,255,0,255);
                    var leg_dist = grtCircDist(altn_wpt_vec[current_page_altn * 3 - 4]);
                    line1Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                    line1Mid.setColor(0,255,0,255);
                    line1Right.setText("");
                    var wpt_id = waypoints.vector[altn_wpt_vec[current_page_altn * 3 - 4]].id;
                    line2Left.setText(wpt_id);
                    line2Left.setColor(0,255,0,255);
                    if (ground_spd < 80){
                        var ete_f = leg_dist / 440;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    } else {
                        var ete_f = leg_dist / ground_spd;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    }
                    line2Mid.setText(ete_str);
                    line2Mid.setColor(0,255,0,255);
                    line2Right.setText("");
                    var hdg = grtCircHdg(altn_wpt_vec[current_page_altn * 3 - 3]);
                    line3Left.setText(sprintf("%03d", hdg) ~ "°");
                    line3Left.setColor(0,255,0,255);
                    var leg_dist = grtCircDist(altn_wpt_vec[current_page_altn * 3 - 3]);
                    line3Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                    line3Mid.setColor(0,255,0,255);
                    line3Right.setText("");
                    var wpt_id = waypoints.vector[altn_wpt_vec[current_page_altn * 3 - 3]].id;
                    line4Left.setText(wpt_id);
                    line4Left.setColor(0,255,0,255);
                    if (ground_spd < 80){
                        var ete_f = leg_dist / 440;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    } else {
                        var ete_f = leg_dist / ground_spd;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    }
                    line4Mid.setText(ete_str);
                    line4Mid.setColor(0,255,0,255);
                    line4Right.setText("");
                    var hdg = grtCircHdg(altn_wpt_vec[current_page_altn * 3 - 2]);
                    line5Left.setText(sprintf("%03d", hdg) ~ "°");
                    line5Left.setColor(0,255,0,255);
                    var leg_dist = grtCircDist(altn_wpt_vec[current_page_altn * 3 - 2]);
                    line5Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                    line5Mid.setColor(0,255,0,255);
                    line5Right.setText("");
                    var wpt_id = waypoints.vector[altn_wpt_vec[current_page_altn * 3 - 2]].id;
                    line6Left.setText(wpt_id);
                    line6Left.setColor(0,255,0,255);
                    if (ground_spd < 80){
                        var ete_f = leg_dist / 440;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    } else {
                        var ete_f = leg_dist / ground_spd;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    }
                    line6Mid.setText(ete_str);
                    line6Mid.setColor(0,255,0,255);
                    line6Right.setText("");

                    var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                    if (L1_press_orig == 0 and press == 1) {
                        if (scratch_pad_message == "DELETE"){
                            var pop_point = non_altn_num + (current_page_altn * 3 - 4);
                            waypoints.pop(pop_point);
                            scratch_pad_message = "";
                        } else {
                            var old_wpt_num = waypoints.size();
                            var insert_point = non_altn_num + (current_page_altn * 3 - 4);
                            read_crt_wpt(scratch_pad_text, insert_point);
                            var new_wpt_num = waypoints.size();
                            if (new_wpt_num > old_wpt_num) {
                                waypoints.vector[wpt_sel_isr].procedure = "altn";
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");
                    
                    var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                    if (L2_press_orig == 0 and press == 1) {
                        if (scratch_pad_message == "DELETE"){
                            var pop_point = non_altn_num + (current_page_altn * 3 - 3);
                            waypoints.pop(pop_point);
                            scratch_pad_message = "";
                        } else {
                            var old_wpt_num = waypoints.size();
                            var insert_point = non_altn_num + (current_page_altn * 3 - 3);
                            read_crt_wpt(scratch_pad_text, insert_point);
                            var new_wpt_num = waypoints.size();
                            if (new_wpt_num > old_wpt_num) {
                                waypoints.vector[wpt_sel_isr].procedure = "altn";
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
                    
                    var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                    if (L3_press_orig == 0 and press == 1) {
                        if (scratch_pad_message == "DELETE"){
                            var pop_point = non_altn_num + (current_page_altn * 3 - 2);
                            waypoints.pop(pop_point);
                            scratch_pad_message = "";
                        } else {
                            var old_wpt_num = waypoints.size();
                            var insert_point = non_altn_num + (current_page_altn * 3 - 2);
                            read_crt_wpt(scratch_pad_text, insert_point);
                            var new_wpt_num = waypoints.size();
                            if (new_wpt_num > old_wpt_num) {
                                waypoints.vector[wpt_sel_isr].procedure = "altn";
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");
                }

                if (altn_wpt_num >= 2 and current_page_prop == total_page_prop and math.fmod(altn_wpt_num, 3) == 2) {
                    line1Left.setText("VIA.TO");
                    line1Left.setColor(255,255,255,255);
                    line1Mid.setText("");
                    line1Right.setText("ALTN");
                    line1Right.setColor(255,255,255,255);
                    line2Left.setText("-----");
                    line2Left.setColor(0,174,255,255);
                    line2Mid.setText("");
                    line2Right.setText(altn_airport);
                    line2Right.setColor(0,255,0,255);
                    line3Left.setText("");
                    line3Mid.setText("");
                    line3Right.setText("");
                    line4Left.setText("");
                    line4Mid.setText("");
                    line4Right.setText("");
                    line5Left.setText("");
                    line5Mid.setText("");
                    line5Right.setText("");
                    line6Left.setText("");
                    line6Mid.setText("");
                    line6Right.setText("");

                    var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                    if (L1_press_orig == 0 and press == 1) {
                        var has_dot = find(".", scratch_pad_text)!=-1;
                        var long_enough = size(scratch_pad_text) >=5;
                        if (has_dot and long_enough) {
                            var is_awy = awy_parser(scratch_pad_text);
                        } else {
                            var is_awy = 0;
                        }
                        if (is_awy == 1) {
                            var insert_point = non_altn_num + (current_page_altn * 3 - 4);
                            for (i=size(awy_wpt_list)-1;i>0;i=i-1) {
                                var awy_wp = createWaypoint.new(awy_wpt_list[i], awy_wpt_list_lat[i], awy_wpt_list_lon[i]);
                                awy_ap.procedure = "altn";
                                waypoints.insert(insert_point, awy_wp);
                            }
                            scratch_pad_text = "";
                        } else {
                            if (scratch_pad_text == altn_airport) {
                                var altn_apt_hash = createWaypoint.new(altn_airport, airportinfo(altn_airport).lat, airportinfo(altn_airport).lon);
                                altn_apt_hash.altitude = airportinfo(altn_airport).elevation;
                                altn_apt_hash.procedure = "altn";
                                waypoints.append(altn_apt_hash);
                            } else {
                                var old_wpt_num = waypoints.size();
                                var insert_point = non_altn_num + (current_page_altn * 3 - 4);
                                read_crt_wpt(scratch_pad_text, insert_point);
                                var new_wpt_num = waypoints.size();
                                if (new_wpt_num > old_wpt_num) {
                                    waypoints.vector[wpt_sel_isr].procedure = "altn";
                                }
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");
                }
                
                if (altn_wpt_num >= 2 and current_page_prop == total_page_prop and math.fmod(altn_wpt_num, 3) == 0) {
                    var hdg = grtCircHdg(altn_wpt_vec[current_page_altn * 3 - 4]);
                    line1Left.setText(sprintf("%03d", hdg) ~ "°");
                    line1Left.setColor(0,255,0,255);
                    var leg_dist = grtCircDist(altn_wpt_vec[current_page_altn * 3 - 4]);
                    line1Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                    line1Mid.setColor(0,255,0,255);
                    line1Right.setText("");
                    var wpt_id = waypoints.vector[altn_wpt_vec[current_page_altn * 3 - 4]].id;
                    line2Left.setText(wpt_id);
                    line2Left.setColor(0,255,0,255);
                    if (ground_spd < 80){
                        var ete_f = leg_dist / 440;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    } else {
                        var ete_f = leg_dist / ground_spd;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    }
                    line2Mid.setText(ete_str);
                    line2Mid.setColor(0,255,0,255);
                    line2Right.setText("");
                    line3Left.setText("VIA.TO");
                    line3Left.setColor(255,255,255,255);
                    line3Mid.setText("");
                    line3Right.setText("ALTN");
                    line3Right.setColor(255,255,255,255);
                    line4Left.setText("-----");
                    line4Left.setColor(0,174,255,255);
                    line4Mid.setText("");
                    line4Right.setText(altn_airport);
                    line4Right.setColor(0,255,0,255);
                    line5Left.setText("");
                    line5Mid.setText("");
                    line5Right.setText("");
                    line6Left.setText("");
                    line6Mid.setText("");
                    line6Right.setText("");

                    var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                    if (L1_press_orig == 0 and press == 1) {
                        if (scratch_pad_message == "DELETE"){
                            var pop_point = non_altn_num + (current_page_altn * 3 - 4);
                            waypoints.pop(pop_point);
                            scratch_pad_message = "";
                        } else {
                            var old_wpt_num = waypoints.size();
                            var insert_point = non_altn_num + (current_page_altn * 3 - 4);
                            read_crt_wpt(scratch_pad_text, insert_point);
                            var new_wpt_num = waypoints.size();
                            if (new_wpt_num > old_wpt_num) {
                                waypoints.vector[wpt_sel_isr].procedure = "altn";
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");

                    var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                    if (L2_press_orig == 0 and press == 1) {
                        var has_dot = find(".", scratch_pad_text)!=-1;
                        var long_enough = size(scratch_pad_text) >=5;
                        if (has_dot and long_enough) {
                            var is_awy = awy_parser(scratch_pad_text);
                        } else {
                            var is_awy = 0;
                        }
                        if (is_awy == 1) {
                            var insert_point = non_altn_num + (current_page_altn * 3 - 3);
                            for (i=size(awy_wpt_list)-1;i>0;i=i-1) {
                                var awy_wp = createWaypoint.new(awy_wpt_list[i], awy_wpt_list_lat[i], awy_wpt_list_lon[i]);
                                awy_ap.procedure = "altn";
                                waypoints.insert(insert_point, awy_wp);
                            }
                            scratch_pad_text = "";
                        } else {
                            if (scratch_pad_text == altn_airport) {
                                var altn_apt_hash = createWaypoint.new(altn_airport, airportinfo(altn_airport).lat, airportinfo(altn_airport).lon);
                                altn_apt_hash.altitude = airportinfo(altn_airport).elevation;
                                altn_apt_hash.procedure = "altn";
                                waypoints.append(altn_apt_hash);
                            } else {
                                var old_wpt_num = waypoints.size();
                                var insert_point = non_altn_num + (current_page_altn * 3 - 3);
                                read_crt_wpt(scratch_pad_text, insert_point);
                                var new_wpt_num = waypoints.size();
                                if (new_wpt_num > old_wpt_num) {
                                    waypoints.vector[wpt_sel_isr].procedure = "altn";
                                }
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
                }
                
                if (altn_wpt_num >= 2 and current_page_prop == total_page_prop and math.fmod(altn_wpt_num, 3) == 1) {
                    var hdg = grtCircHdg(altn_wpt_vec[current_page_altn * 3 - 4]);
                    line1Left.setText(sprintf("%03d", hdg) ~ "°");
                    line1Left.setColor(0,255,0,255);
                    var leg_dist = grtCircDist(altn_wpt_vec[current_page_altn * 3 - 4]);
                    line1Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                    line1Mid.setColor(0,255,0,255);
                    line1Right.setText("");
                    var wpt_id = waypoints.vector[altn_wpt_vec[current_page_altn * 3 - 4]].id;
                    line2Left.setText(wpt_id);
                    line2Left.setColor(0,255,0,255);
                    if (ground_spd < 80){
                        var ete_f = leg_dist / 440;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    } else {
                        var ete_f = leg_dist / ground_spd;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    }
                    line2Mid.setText(ete_str);
                    line2Mid.setColor(0,255,0,255);
                    line2Right.setText("");
                    var hdg = grtCircHdg(altn_wpt_vec[current_page_altn * 3 - 3]);
                    line3Left.setText(sprintf("%03d", hdg) ~ "°");
                    line3Left.setColor(0,255,0,255);
                    var leg_dist = grtCircDist(altn_wpt_vec[current_page_altn * 3 - 3]);
                    line3Mid.setText(sprintf("%4.0f", leg_dist) ~ "NM");
                    line3Mid.setColor(0,255,0,255);
                    line3Right.setText("");
                    var wpt_id = waypoints.vector[altn_wpt_vec[current_page_altn * 3 - 3]].id;
                    line4Left.setText(wpt_id);
                    line4Left.setColor(0,255,0,255);
                    if (ground_spd < 80){
                        var ete_f = leg_dist / 440;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    } else {
                        var ete_f = leg_dist / ground_spd;
                        var ete_h = sprintf("%02d", ete_f);
                        var ete_m = sprintf("%02d", (ete_f-math.floor(ete_f))*60);
                        var ete_str = ete_h ~ " + " ~ ete_m;
                    }
                    line4Mid.setText(ete_str);
                    line4Mid.setColor(0,255,0,255);
                    line4Right.setText("");
                    line5Left.setText("VIA.TO");
                    line5Left.setColor(255,255,255,255);
                    line5Mid.setText("");
                    line5Right.setText("ALTN");
                    line5Right.setColor(255,255,255,255);
                    line6Left.setText("-----");
                    line6Left.setColor(0,174,255,255);
                    line6Mid.setText("");
                    line6Right.setText(altn_airport);
                    line6Right.setColor(0,255,0,255);
                    var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                    if (L1_press_orig == 0 and press == 1) {
                        if (scratch_pad_message == "DELETE"){
                            var pop_point = non_altn_num + (current_page_altn * 3 - 4);
                            waypoints.pop(pop_point);
                            scratch_pad_message = "";
                        } else {
                            var old_wpt_num = waypoints.size();
                            var insert_point = non_altn_num + (current_page_altn * 3 - 4);
                            read_crt_wpt(scratch_pad_text, insert_point);
                            var new_wpt_num = waypoints.size();
                            if (new_wpt_num > old_wpt_num) {
                                waypoints.vector[wpt_sel_isr].procedure = "altn";
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");

                    var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                    if (L2_press_orig == 0 and press == 1) {
                        if (scratch_pad_message == "DELETE"){
                            var pop_point = non_altn_num + (current_page_altn * 3 - 3);
                            waypoints.pop(pop_point);
                            scratch_pad_message = "";
                        } else {
                            var old_wpt_num = waypoints.size();
                            var insert_point = non_altn_num + (current_page_altn * 3 - 3);
                            read_crt_wpt(scratch_pad_text, insert_point);
                            var new_wpt_num = waypoints.size();
                            if (new_wpt_num > old_wpt_num) {
                                waypoints.vector[wpt_sel_isr].procedure = "altn";
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
                    var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                    if (L3_press_orig == 0 and press == 1) {
                        var has_dot = find(".", scratch_pad_text)!=-1;
                        var long_enough = size(scratch_pad_text) >=5;
                        if (has_dot and long_enough) {
                            var is_awy = awy_parser(scratch_pad_text);
                        } else {
                            var is_awy = 0;
                        }
                        if (is_awy == 1) {
                            var insert_point = non_altn_num + (current_page_altn * 3 - 2);
                            for (i=size(awy_wpt_list)-1;i>0;i=i-1) {
                                var awy_wp = createWaypoint.new(awy_wpt_list[i], awy_wpt_list_lat[i], awy_wpt_list_lon[i]);
                                awy_ap.procedure = "altn";
                                waypoints.insert(insert_point, awy_wp);
                            }
                            scratch_pad_text = "";
                        } else {
                            if (scratch_pad_text == altn_airport) {
                                var altn_apt_hash = createWaypoint.new(altn_airport, airportinfo(altn_airport).lat, airportinfo(altn_airport).lon);
                                altn_apt_hash.altitude = airportinfo(altn_airport).elevation;
                                altn_apt_hash.procedure = "altn";
                                waypoints.append(altn_apt_hash);
                            } else {
                                var old_wpt_num = waypoints.size();
                                var insert_point = non_altn_num + (current_page_altn * 3 - 2);
                                read_crt_wpt(scratch_pad_text, insert_point);
                                var new_wpt_num = waypoints.size();
                                if (new_wpt_num > old_wpt_num) {
                                    waypoints.vector[wpt_sel_isr].procedure = "altn";
                                }
                            }
                            scratch_pad_text = ""; 
                        }
                    }
                    L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");
                }
            }
        }
    }
    fmz_setlistener(pageActiveFltPlan);
    var pageActiveFltPlan_timer = maketimer(1, pageActiveFltPlan);
    pageActiveFltPlan_timer.start();

    var pageWayPointSelect = func {
        if (page_title_text == "WAYPOINT SELECT") {
            eachPageInit();
            total_page_prop = math.ceil(size(wpt_sel_vec) / 3);
            pageTitle.setText(sprintf("%s", "WAYPOINT SELECT"));
            pageTitle.setColor(255,255,255,255);
            pageTitleRight.setText("");
            currentPage.setText(sprintf("%d", current_page_prop));
            currentPage.setColor(255,255,255,255);
            totalPage.setText(sprintf("%d", total_page_prop));
            totalPage.setColor(255,255,255,255);
            line1Left.setText(wpt_sel_vec[current_page_prop*3-3][0]);
            line1Left.setColor(255,255,255,255);
            line1Mid.setText(sprintf("%.0f", wpt_sel_vec[current_page_prop*3-3][3]) ~ "NM");
            line1Mid.setColor(255,255,255,255);
            line1Right.setText("RETURN ▶");

            var wpt_pos = "";
            if (wpt_sel_vec[current_page_prop*3-3][1]<0) {
                wpt_pos = wpt_pos ~ "◀ S";
            } else {
                wpt_pos = wpt_pos ~ "◀ N";
            }
            var lat_deg = math.floor(abs(wpt_sel_vec[current_page_prop*3-3][1]));
            var lat_min = (abs(wpt_sel_vec[current_page_prop*3-3][1])-lat_deg)*60;
            lat_deg = sprintf("%02d", lat_deg);
            lat_min = sprintf("%05.2f", lat_min);
            wpt_pos = wpt_pos ~ lat_deg ~ "° " ~ lat_min;
            if (wpt_sel_vec[current_page_prop*3-3][2]<0) {
                wpt_pos = wpt_pos ~ " W";
            } else {
                wpt_pos = wpt_pos ~ " E";
            }
            var lon_deg = math.floor(abs(wpt_sel_vec[current_page_prop*3-3][2]));
            var lon_min = (abs(wpt_sel_vec[current_page_prop*3-3][2])-lon_deg) * 60;
            lon_deg = sprintf("%03d", lon_deg);
            lon_min = sprintf("%05.2f", lon_min);
            wpt_pos = wpt_pos ~ lon_deg ~ "° " ~ lon_min;

            line2Left.setText(wpt_pos);
            line2Left.setColor(0,255,0,255);
            line2Mid.setText("");
            line2Right.setText("");

            if (math.fmod(size(wpt_sel_vec),3)==2 or math.fmod(size(wpt_sel_vec),3)==0 or current_page_prop < total_page_prop) {
                line3Left.setText(wpt_sel_vec[current_page_prop*3-2][0]);
                line3Left.setColor(255,255,255,255);
                line3Mid.setText(sprintf("%.0f", wpt_sel_vec[current_page_prop*3-2][3]) ~ "NM");
                line3Mid.setColor(255,255,255,255);
                line3Right.setText("");

                var wpt_pos = "";
                if (wpt_sel_vec[current_page_prop*3-2][1]<0) {
                    wpt_pos = wpt_pos ~ "◀ S";
                } else {
                    wpt_pos = wpt_pos ~ "◀ N";
                }
                var lat_deg = math.floor(abs(wpt_sel_vec[current_page_prop*3-2][1]));
                var lat_min = (abs(wpt_sel_vec[current_page_prop*3-2][1])-lat_deg)*60;
                lat_deg = sprintf("%02d", lat_deg);
                lat_min = sprintf("%04.2f", lat_min);
                wpt_pos = wpt_pos ~ lat_deg ~ "° " ~ lat_min;
                if (wpt_sel_vec[current_page_prop*3-2][2]<0) {
                    wpt_pos = wpt_pos ~ " W";
                } else {
                    wpt_pos = wpt_pos ~ " E";
                }
                var lon_deg = math.floor(abs(wpt_sel_vec[current_page_prop*3-2][2]));
                var lon_min = (abs(wpt_sel_vec[current_page_prop*3-2][2])-lon_deg) * 60;
                lon_deg = sprintf("%03d", lon_deg);
                lon_min = sprintf("%05.2f", lon_min);
                wpt_pos = wpt_pos ~ lon_deg ~ "° " ~ lon_min;

                line4Left.setText(wpt_pos);
                line4Left.setColor(0,255,0,255);
                line4Mid.setText("");
                line4Right.setText("");

                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1) {
                    var isr_name = wpt_sel_vec[current_page_prop*3-2][0];
                    var isr_lat = wpt_sel_vec[current_page_prop*3-2][1];
                    var isr_lon = wpt_sel_vec[current_page_prop*3-2][2];
                    var wp = createWaypoint.new(isr_name, isr_lat, isr_lon);
                    waypoints.insert(wpt_sel_isr, wp);
                    wpt_num = waypoints.size();
                    var wc = 0;
                    while((abs(waypoints.vector[wc].lat - isr_lat) > 0.0001 or abs(waypoints.vector[wc].lon - isr_lon) > 0.0001) and wc <= wpt_num) {
                        wc = wc + 1;
                    }
                    if (wc <  wpt_sel_isr){
                        for (i=wc; i<(wpt_sel_isr); i=i+1){
                            waypoints.pop(wc);
                        }
                    }
                    if (save_current_page > (total_page_rte + total_page_miss)){
                        waypoints.vector[wpt_sel_isr].procedure = "altn";
                    }
                    page_title_text = "ACTIVE FLT PLAN";
                    current_page_prop = save_current_page;
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

            } else {
                line3Left.setText("");
                line3Mid.setText("");
                line3Right.setText("");
                line4Left.setText("");
                line4Mid.setText("");
                line4Right.setText("");
            }

            if (math.fmod(size(wpt_sel_vec),3)==3 or current_page_prop < total_page_prop) {
                line5Left.setText(wpt_sel_vec[current_page_prop*3-1][0]);
                line5Left.setColor(255,255,255,255);
                line5Mid.setText(sprintf("%.0f", wpt_sel_vec[current_page_prop*3-1][3]) ~ "NM");
                line5Mid.setColor(255,255,255,255);
                line3Mid.setColor(255,255,255,255);
                line3Right.setText("");

                var wpt_pos = "";
                if (wpt_sel_vec[current_page_prop*3-1][1]<0) {
                    wpt_pos = wpt_pos ~ "◀ S";
                } else {
                    wpt_pos = wpt_pos ~ "◀ N";
                }
                var lat_deg = math.floor(abs(wpt_sel_vec[current_page_prop*3-1][1]));
                var lat_min = (abs(wpt_sel_vec[current_page_prop*3-1][1])-lat_deg)*60;
                lat_deg = sprintf("%02d", lat_deg);
                lat_min = sprintf("%05.2f", lat_min);
                wpt_pos = wpt_pos ~ lat_deg ~ "° " ~ lat_min;
                if (wpt_sel_vec[current_page_prop*3-1][2]<0) {
                    wpt_pos = wpt_pos ~ " W";
                } else {
                    wpt_pos = wpt_pos ~ " E";
                }
                var lon_deg = math.floor(abs(wpt_sel_vec[current_page_prop*3-1][2]));
                var lon_min = (abs(wpt_sel_vec[current_page_prop*3-1][2])-lon_deg) * 60;
                lon_deg = sprintf("%03d", lon_deg);
                lon_min = sprintf("%05.2f", lon_min);
                wpt_pos = wpt_pos ~ lon_deg ~ "° " ~ lon_min;

                line6Left.setText(wpt_pos);
                line6Left.setColor(0,255,0,255);
                line6Mid.setText("");
                line6Right.setText("");

                var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                if (L3_press_orig == 0 and press == 1) {
                    var isr_name = wpt_sel_vec[current_page_prop*3-1][0];
                    var isr_lat = wpt_sel_vec[current_page_prop*3-1][1];
                    var isr_lon = wpt_sel_vec[current_page_prop*3-1][2];
                    var wp = createWaypoint.new(isr_name, isr_lat, isr_lon);
                    waypoints.insert(wpt_sel_isr, wp);
                    wpt_num = waypoints.size();
                    var wc = 0;
                    while((abs(waypoints.vector[wc].lat - isr_lat) > 0.0001 or abs(waypoints.vector[wc].lon - isr_lon) > 0.0001) and wc <= wpt_num) {
                        wc = wc + 1;
                    }
                    if (wc <  wpt_sel_isr){
                        for (i=wc; i<(wpt_sel_isr); i=i+1){
                            waypoints.pop(wc);
                        }
                    }
                    if (save_current_page > (total_page_rte + total_page_miss)){
                        waypoints.vector[wpt_sel_isr].procedure = "altn";
                    }
                    page_title_text = "ACTIVE FLT PLAN";
                    current_page_prop = save_current_page;
                }
                L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");

            } else {
                line5Left.setText("");
                line5Mid.setText("");
                line5Right.setText("");
                line6Left.setText("");
                line6Mid.setText("");
                line6Right.setText("");
            }
            line7Left.setText("");
            line7Mid.setText("");
            line7Right.setText("");

            var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
            if (R1_press_orig == 0 and press == 1) {
                page_title_text = "ACTIVE FLT PLAN";
                current_page_prop = save_current_page;
            }
            R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");

            var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
            if (L1_press_orig == 0 and press == 1) {
                var isr_name = wpt_sel_vec[current_page_prop*3-3][0];
                var isr_lat = wpt_sel_vec[current_page_prop*3-3][1];
                var isr_lon = wpt_sel_vec[current_page_prop*3-3][2];
                var wp = createWaypoint.new(isr_name, isr_lat, isr_lon);
                waypoints.insert(wpt_sel_isr, wp);
                wpt_num = waypoints.size();
                var wc = 0;
                while((abs(waypoints.vector[wc].lat - isr_lat) > 0.0001 or abs(waypoints.vector[wc].lon - isr_lon) > 0.0001) and wc <= wpt_num) {
                    wc = wc + 1;
                }
                if (wc <  wpt_sel_isr){
                    for (i=wc; i<(wpt_sel_isr); i=i+1){
                        waypoints.pop(wc);
                    }
                }
                if (save_current_page > (total_page_rte + total_page_miss)){
                    waypoints.vector[wpt_sel_isr].procedure = "altn";
                }
                page_title_text = "ACTIVE FLT PLAN";
                current_page_prop = save_current_page;
            }
            L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");
        }
    }
    fmz_setlistener(pageWayPointSelect);

    var pageFplPlanSelect = func {   
        
        if (page_title_text == "FPL PLAN SELECT"){
            eachPageInit();
            var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
            if (R1_press_orig == 0 and press == 1) {
                is_active = 1;
                page_title_text = "ACTIVE FLT PLAN";
            }
            R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");
            
            var origin_airport = origin_airport_id;
            pageTitle.setText(sprintf("%s", "FPL PLAN SELECT"));
            pageTitle.setColor(255,255,255,255);
            pageTitleRight.setText(sprintf("%s", ""));
            pageTitleRight.setColor(255,255,255,255);
            currentPage.setText(sprintf("%d", 1));
            currentPage.setColor(255,255,255,255);
            totalPage.setText(sprintf("%d", 1));
            totalPage.setColor(255,255,255,255);
            line1Left.setText(sprintf("%s", "FLT PLAN"));
            line1Left.setColor(255,255,255,255);
            line2Left.setText(sprintf("%s",origin_airport ~ " - " ~ dest_airport));
            line2Left.setColor(255,255,0,255);
            line3Left.setText(sprintf("%s", ""));
            line3Left.setColor(255,255,255,255);
            line4Left.setText(sprintf("%s", ""));
            line4Left.setColor(255,255,255,255);
            line5Left.setText(sprintf("%s", ""));
            line5Left.setColor(255,255,255,255);
            line6Left.setText(sprintf("%s", ""));
            line6Left.setColor(255,255,255,255);    
            line7Left.setText(sprintf("%s", "◀ FPL LIST"));
            line7Left.setColor(255,255,255,255);
            line1Mid.setText(sprintf("%s", ""));
            line1Mid.setColor(255,255,255,255);
            line2Mid.setText(sprintf("%s", ""));
            line2Mid.setColor(255,255,255,255);
            line3Mid.setText(sprintf("%s", ""));
            line3Mid.setColor(255,255,255,255);
            line4Mid.setText(sprintf("%s", ""));
            line4Mid.setColor(255,255,255,255);
            line5Mid.setText(sprintf("%s", ""));
            line5Mid.setColor(255,255,255,255);
            line6Mid.setText(sprintf("%s", ""));
            line6Mid.setColor(255,255,255,255);
            line7Mid.setText(sprintf("%s", ""));
            line7Mid.setColor(255,255,255,255);
            line1Right.setText(sprintf("%s", ""));
            line1Right.setColor(255,255,255,255);
            line2Right.setText(sprintf("%s", "ACTIVATE ▶"));
            line2Right.setColor(255,255,255,255);
            line3Right.setText(sprintf("%s", ""));
            line3Right.setColor(255,255,255,255);
            line4Right.setText(sprintf("%s", "INVERT/ACTIVATE ▶"));
            line4Right.setColor(255,255,255,255);
            line5Right.setText(sprintf("%s", ""));
            line5Right.setColor(255,255,255,255);
            line6Right.setText(sprintf("%s", "STORED FPL PREF ▶"));
            line6Right.setColor(255,255,255,255);
            line7Right.setText(sprintf("%s", ""));
            line7Right.setColor(255,255,255,255);
        }
    }
    fmz_setlistener(pageFplPlanSelect);

    var pageDepartureRunways = func {
        if (page_title_text == "DEPARTURE RUNWAYS") {
            eachPageInit();
            var origin_airport = origin_airport_id;
            var runways = airportinfo(origin_airport).runways;
            var runways_vector = [];
            foreach(var rwy; keys(runways)){
                append(runways_vector, rwy);
            }
            total_page_prop = math.ceil(size(runways)/4);
            var runway_sel = sld_rwy;

            pageTitle.setText(sprintf("%s", "DEPARTURE RUNWAYS"));
            pageTitle.setColor(255,255,255,255);
            pageTitleRight.setText(sprintf("%s", ""));
            pageTitleRight.setColor(255,255,255,255);
            currentPage.setText(sprintf("%d", current_page_prop));
            currentPage.setColor(255,255,255,255);
            totalPage.setText(sprintf("%d",total_page_prop));
            totalPage.setColor(255,255,255,255);
            line1Left.setText(sprintf("%s", ""));
            line1Left.setColor(255,255,255,255);
            line2Left.setText(sprintf("%s", origin_airport));
            line2Left.setColor(0,255,0,255);
            line3Left.setText(sprintf("%s", ""));
            line3Left.setColor(255,255,255,255);

            var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
            if (L2_press_orig == 0 and press == 1) {
                var rwy_num = runways_vector[current_page_prop*4-4];
                sld_rwy = rwy_num;

                var orgn_apt_rwy_hash = createWaypoint.new((origin_airport ~ " -- " ~ rwy_num), airportinfo(origin_airport).runways[rwy_num].lat, airportinfo(origin_airport).runways[rwy_num].lon);
                orgn_apt_rwy_hash.altitude = airportinfo(origin_airport).elevation;
                waypoints.pop(0);
                waypoints.insert(0, orgn_apt_rwy_hash);

                var rwy_sids = listSids(origin_airport, rwy_num);
                if (size(rwy_sids)>0){
                    page_title_text = "SIDs";
                    current_page_prop = 1;
                }
            }
            L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

            var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
            if (R2_press_orig == 0 and press == 1) {
                var rwy_num = runways_vector[current_page_prop*4-3];
                sld_rwy = rwy_num;

                var orgn_apt_rwy_hash = createWaypoint.new((origin_airport ~ " -- " ~ rwy_num), airportinfo(origin_airport).runways[rwy_num].lat, airportinfo(origin_airport).runways[rwy_num].lon);
                orgn_apt_rwy_hash.altitude = airportinfo(origin_airport).elevation;
                waypoints.pop(0);
                waypoints.insert(0, orgn_apt_rwy_hash);

                var rwy_sids = listSids(origin_airport, rwy_num);
                if (size(rwy_sids)>0){
                    page_title_text = "SIDs";
                    current_page_prop = 1;
                }
            }
            R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");

            var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
            if (L3_press_orig == 0 and press == 1 and (math.fmod(size(runways_vector),4) == 0 or current_page_prop < total_page_prop)) {
                var rwy_num = runways_vector[current_page_prop*4-2];
                sld_rwy = rwy_num;

                var orgn_apt_rwy_hash = createWaypoint.new((origin_airport ~ " -- " ~ rwy_num), airportinfo(origin_airport).runways[rwy_num].lat, airportinfo(origin_airport).runways[rwy_num].lon);
                orgn_apt_rwy_hash.altitude = airportinfo(origin_airport).elevation;
                waypoints.pop(0);
                waypoints.insert(0, orgn_apt_rwy_hash);

                var rwy_sids = listSids(origin_airport, rwy_num);
                if (size(rwy_sids)>0){
                    page_title_text = "SIDs";
                    current_page_prop = 1;
                }
            }
            L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");

            var press = getprop("/instrumentation/fmz/fmz-key-R3-press");
            if (R3_press_orig == 0 and press == 1 and (math.fmod(size(runways_vector),4) == 0 or current_page_prop < total_page_prop)) {
                var rwy_num = runways_vector[current_page_prop*4-1];
                sld_rwy = rwy_num;

                var orgn_apt_rwy_hash = createWaypoint.new((origin_airport ~ " -- " ~ rwy_num), airportinfo(origin_airport).runways[rwy_num].lat, airportinfo(origin_airport).runways[rwy_num].lon);
                orgn_apt_rwy_hash.altitude = airportinfo(origin_airport).elevation;
                waypoints.pop(0);
                waypoints.insert(0, orgn_apt_rwy_hash);

                var rwy_sids = listSids(origin_airport, rwy_num);
                if (size(rwy_sids)>0){
                    page_title_text = "SIDs";
                    current_page_prop = 1;
                }
            }
            R3_press_orig = getprop("/instrumentation/fmz/fmz-key-R3-press");

            if (runways_vector[current_page_prop*4-4] == runway_sel){
                line4Left.setText(sprintf("%s", runways_vector[current_page_prop*4-4] ~ " [SEL]"));
            } else {
                line4Left.setText(sprintf("%s", runways_vector[current_page_prop*4-4]));
            }
            line4Left.setColor(0,255,0,255);
            line5Left.setText(sprintf("%s", ""));
            line5Left.setColor(255,255,255,255);
            if (runways_vector[current_page_prop*4-3] == runway_sel){
                line4Right.setText(sprintf("%s", runways_vector[current_page_prop*4-3] ~ " [SEL]"));
            } else {
                line4Right.setText(sprintf("%s", runways_vector[current_page_prop*4-3]));
            }
            line6Left.setColor(0,255,0,255);    
            line7Left.setText(sprintf("%s", ""));
            line7Left.setColor(255,255,255,255);
            line1Mid.setText(sprintf("%s", ""));
            line1Mid.setColor(255,255,255,255);
            line2Mid.setText(sprintf("%s", ""));
            line2Mid.setColor(255,255,255,255);
            line3Mid.setText(sprintf("%s", ""));
            line3Mid.setColor(255,255,255,255);
            line4Mid.setText(sprintf("%s", ""));
            line4Mid.setColor(255,255,255,255);
            line5Mid.setText(sprintf("%s", ""));
            line5Mid.setColor(255,255,255,255);
            line6Mid.setText(sprintf("%s", ""));
            line6Mid.setColor(255,255,255,255);
            line7Mid.setText(sprintf("%s", ""));
            line7Mid.setColor(255,255,255,255);
            line1Right.setText(sprintf("%s", ""));
            line1Right.setColor(255,255,255,255);
            line2Right.setText(sprintf("%s", ""));
            line2Right.setColor(255,255,255,255);
            line3Right.setText(sprintf("%s", ""));
            line3Right.setColor(255,255,255,255);
            if (current_page_prop < total_page_prop){
                if(runways_vector[current_page_prop*4-2] == runway_sel){
                    line6Left.setText(sprintf("%s", "[SEL] " ~ runways_vector[current_page_prop*4-2]));
                } else {
                    line6Left.setText(sprintf("%s", runways_vector[current_page_prop*4-2]));
                }
                if(runways_vector[current_page_prop*4-1] == runway_sel){
                    line6Right.setText(sprintf("%s", "[SEL] " ~ runways_vector[current_page_prop*4-1]));
                } else {
                    line6Right.setText(sprintf("%s", runways_vector[current_page_prop*4-1]));
                }
            } else if (math.fmod(size(runways),4)==0){
                if(runways_vector[current_page_prop*4-2] == runway_sel){
                    line6Left.setText(sprintf("%s", "[SEL] " ~ runways_vector[current_page_prop*4-2]));
                } else {
                    line6Left.setText(sprintf("%s", runways_vector[current_page_prop*4-2]));
                }
                if(runways_vector[current_page_prop*4-1] == runway_sel){
                    line6Right.setText(sprintf("%s", "[SEL] " ~ runways_vector[current_page_prop*4-1]));
                } else {
                    line6Right.setText(sprintf("%s", runways_vector[current_page_prop*4-1]));
                }
            } else {
                line6Left.setText(sprintf("%s", ""));
                line6Right.setText(sprintf("%s", ""));
            }
            line4Right.setColor(0,255,0,255);
            line5Right.setText(sprintf("%s", ""));
            line5Right.setColor(255,255,255,255);
            
            line6Right.setColor(0,255,0,255);
            line7Right.setText(sprintf("%s", "TAKEOFF ▶"));
            line7Right.setColor(255,255,255,255);            
        }

    }
    fmz_setlistener(pageDepartureRunways);

    var pageSids = func {   
        
        if (page_title_text == "SIDs"){
            eachPageInit();
            var origin_airport = origin_airport_id;
            var rwy_num = sld_rwy;
            var rwy_sids = listSids(origin_airport, rwy_num);
            total_page_prop = math.ceil(size(rwy_sids)/4);

            var press = getprop("/instrumentation/fmz/fmz-key-R4-press");
            if (R4_press_orig == 0 and press == 1) {
                is_active = 1;
                page_title_text = "ACTIVE FLT PLAN";
                current_page_prop = 1;
            }
            R4_press_orig = getprop("/instrumentation/fmz/fmz-key-R4-press");
            
            pageTitle.setText(sprintf("%s", "SIDs"));
            pageTitle.setColor(255,255,255,255);
            pageTitleRight.setText(sprintf("%s", ""));
            pageTitleRight.setColor(255,255,255,255);
            currentPage.setText(sprintf("%d", current_page_prop));
            currentPage.setColor(255,255,255,255);
            totalPage.setText(sprintf("%d",total_page_prop));
            line1Left.setText(sprintf("%s", ""));
            line1Left.setColor(255,255,255,255);
            line2Left.setText(sprintf("%s", origin_airport ~ " " ~ rwy_num));
            line2Left.setColor(0,255,0,255);
            line3Left.setText(sprintf("%s", ""));
            line3Left.setColor(255,255,255,255);
            if (sel_sid != rwy_sids[current_page_prop*4-4]) {
                line4Left.setText(sprintf("%s", rwy_sids[current_page_prop*4-4]));
            } else {
                line4Left.setText(sprintf("%s", rwy_sids[current_page_prop*4-4] ~ " [SEL]"));
            }
            line4Left.setColor(0,255,0,255);
            var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
            if (L2_press_orig == 0 and press == 1) {
                isrSids(origin_airport, rwy_sids[current_page_prop*4-4]);
                if (size(listSidTran()) > 0) {
                    page_title_text = "SID TRANSITIONS";
                    current_page_prop = 1;
                }
            }
            L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

            line5Left.setText(sprintf("%s", ""));
            line5Left.setColor(255,255,255,255);
            if (math.fmod(size(rwy_sids),4) == 2 or math.fmod(size(rwy_sids),4) == 3 or math.fmod(size(rwy_sids),4) == 0 or current_page_prop < total_page_prop) {
                if (sel_sid != rwy_sids[current_page_prop*4-3]) {
                    line4Right.setText(sprintf("%s", rwy_sids[current_page_prop*4-3]));
                } else {
                    line4Right.setText(sprintf("%s", rwy_sids[current_page_prop*4-3] ~ " [SEL]"));
                }
                var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
                if (R2_press_orig == 0 and press == 1) {
                    isrSids(origin_airport, rwy_sids[current_page_prop*4-3]);
                    if (size(listSidTran()) > 0) {
                        page_title_text = "SID TRANSITIONS";
                        current_page_prop = 1;
                    }
                }
                R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");
            } else {
                line4Right.setText(sprintf("%s", ""));
            }
            line6Left.setColor(0,255,0,255);    
            line7Left.setText(sprintf("%s", ""));
            line7Left.setColor(255,255,255,255);
            line1Mid.setText(sprintf("%s", ""));
            line1Mid.setColor(255,255,255,255);
            line2Mid.setText(sprintf("%s", ""));
            line2Mid.setColor(255,255,255,255);
            line3Mid.setText(sprintf("%s", ""));
            line3Mid.setColor(255,255,255,255);
            line4Mid.setText(sprintf("%s", ""));
            line4Mid.setColor(255,255,255,255);
            line5Mid.setText(sprintf("%s", ""));
            line5Mid.setColor(255,255,255,255);
            line6Mid.setText(sprintf("%s", ""));
            line6Mid.setColor(255,255,255,255);
            line7Mid.setText(sprintf("%s", ""));
            line7Mid.setColor(255,255,255,255);
            line1Right.setText(sprintf("%s", ""));
            line1Right.setColor(255,255,255,255);
            line2Right.setText(sprintf("%s", ""));
            line2Right.setColor(255,255,255,255);
            line3Right.setText(sprintf("%s", ""));
            line3Right.setColor(255,255,255,255);
            if (math.fmod(size(rwy_sids),4) == 3 or math.fmod(size(rwy_sids),4) == 0 or current_page_prop < total_page_prop) {
                if (sel_sid != rwy_sids[current_page_prop*4-2]) {
                    line6Left.setText(sprintf("%s", rwy_sids[current_page_prop*4-2]));
                } else {
                    line6Left.setText(sprintf("%s", "[SEL] " ~ rwy_sids[current_page_prop*4-2]));
                }
                var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                if (L3_press_orig == 0 and press == 1) {
                    isrSids(origin_airport, rwy_sids[current_page_prop*4-2]);
                    if (size(listSidTran()) > 0) {
                        page_title_text = "SID TRANSITIONS";
                        current_page_prop = 1;
                    }
                }
                L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");
            } else {
                line6Left.setText(sprintf("%s", ""));
            }
            line4Right.setColor(0,255,0,255);
            line5Right.setText(sprintf("%s", ""));
            line5Right.setColor(255,255,255,255);
            if (math.fmod(size(rwy_sids),4) == 0 or current_page_prop < total_page_prop) {
                if (sel_sid != rwy_sids[current_page_prop*4-1]) {
                    line6Right.setText(sprintf("%s", rwy_sids[current_page_prop*4-1]));
                } else {
                    line6Right.setText(sprintf("%s", "[SEL] " ~ rwy_sids[current_page_prop*4-1]));
                }
                var press = getprop("/instrumentation/fmz/fmz-key-R3-press");
                if (R3_press_orig == 0 and press == 1) {
                    isrSids(origin_airport, rwy_sids[current_page_prop*4-1]);
                    if (size(listSidTran()) > 0) {
                        page_title_text = "SID TRANSITIONS";
                        current_page_prop = 1;
                    }
                }
                R3_press_orig = getprop("/instrumentation/fmz/fmz-key-R3-press");

            } else {
                line6Right.setText(sprintf("%s", ""));
            }
            line6Right.setColor(0,255,0,255);
            line7Right.setText(sprintf("%s", "ACTIVATE ▶"));
            line7Right.setColor(255,255,255,255);
        }
    }
    fmz_setlistener(pageSids);

    var pageSidTran = func () {
        if (page_title_text == "SID TRANSITIONS") {
            eachPageInit();
            var sid_tran_size = size(listSidTran());
            var sid_tran_list = listSidTran();
            total_page_prop = math.ceil(sid_tran_size / 4);

            var press = getprop("/instrumentation/fmz/fmz-key-R4-press");
            if (R4_press_orig == 0 and press == 1) {
                is_active = 1;
                page_title_text = "ACTIVE FLT PLAN";
                current_page_prop = 1;
            }
            R4_press_orig = getprop("/instrumentation/fmz/fmz-key-R4-press");

            currentPage.setText(sprintf("%d", current_page_prop));
            currentPage.setColor(255,255,255,255);
            totalPage.setText(sprintf("%d",total_page_prop));
            totalPage.setColor(255,255,255,255);
            pageTitle.setText("SID TRANSITIONS");
            pageTitle.setColor(255,255,255,255);
            pageTitleRight.setText("");
            line1Left.setText("");
            line2Left.setText(sprintf("%s", sel_sid));
            line2Left.setColor(0,255,0,255);

            if (current_page_prop == total_page_prop and math.fmod(sid_tran_size, 4) == 1) {

                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1) {
                    isrSidTran(sid_tran_list[current_page_prop * 4 - 4]);
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

                if (sel_sid_tran == sid_tran_list[current_page_prop * 4 - 4]) {
                    line4Left.setText(sid_tran_list[current_page_prop * 4 - 4] ~ " [SEL]");
                } else {
                    line4Left.setText(sid_tran_list[current_page_prop * 4 - 4]);
                }
                line4Left.setColor(0,255,0,255);
                line6Left.setText("");
                line4Right.setText("");
                line6Right.setText("");
            }

            if (current_page_prop == total_page_prop and math.fmod(sid_tran_size, 4) == 2) {

                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1) {
                    isrSidTran(sid_tran_list[current_page_prop * 4 - 4]);
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
                if (R2_press_orig == 0 and press == 1) {
                    isrSidTran(sid_tran_list[current_page_prop * 4 - 3]);
                }
                R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");
                
                if (sel_sid_tran == sid_tran_list[current_page_prop * 4 - 4]) {
                    line4Left.setText(sid_tran_list[current_page_prop * 4 - 4] ~ " [SEL]");
                } else {
                    line4Left.setText(sid_tran_list[current_page_prop * 4 - 4]);
                }
                line4Left.setColor(0,255,0,255);

                if (sel_sid_tran == sid_tran_list[current_page_prop * 4 - 3]){
                    line4Right.setText("[SEL] " ~ sid_tran_list[current_page_prop * 4 - 3]);
                } else {
                    line4Right.setText(sid_tran_list[current_page_prop * 4 - 3]);
                }               
                line4Right.setColor(0,255,0,255);
                line4Left.setText("");
                line6Right.setText("");
            }

            if (current_page_prop == total_page_prop and math.fmod(sid_tran_size, 4) == 3) {

                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1) {
                    isrSidTran(sid_tran_list[current_page_prop * 4 - 4]);
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
                if (R2_press_orig == 0 and press == 1) {
                    isrSidTran(sid_tran_list[current_page_prop * 4 - 3]);
                }
                R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                if (L3_press_orig == 0 and press == 1) {
                    isrSidTran(sid_tran_list[current_page_prop * 4 - 2]);
                }
                L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");
                if (sel_sid_tran == sid_tran_list[current_page_prop * 4 - 4]) {
                    line4Left.setText(sid_tran_list[current_page_prop * 4 - 4] ~ " [SEL]");
                } else {
                    line4Left.setText(sid_tran_list[current_page_prop * 4 - 4]);
                }
                line4Left.setColor(0,255,0,255);
                if (sel_sid_tran == sid_tran_list[current_page_prop * 4 - 2]) {
                    line6Left.setText(sid_tran_list[current_page_prop * 4 - 2] ~ " [SEL]");
                } else {
                    line6Left.setText(sid_tran_list[current_page_prop * 4 - 2]);
                }
                line6Left.setColor(0,255,0,255);
                if (sel_sid_tran == sid_tran_list[current_page_prop * 4 - 3]){
                    line4Right.setText("[SEL] " ~ sid_tran_list[current_page_prop * 4 - 3]);
                } else {
                    line4Right.setText(sid_tran_list[current_page_prop * 4 - 3]);
                }               
                line4Right.setColor(0,255,0,255);
                line6Right.setText("");
            }

            if (current_page_prop == total_page_prop and math.fmod(sid_tran_size, 4) == 0) {

                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1) {
                    isrSidTran(sid_tran_list[current_page_prop * 4 - 4]);
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
                if (R2_press_orig == 0 and press == 1) {
                    isrSidTran(sid_tran_list[current_page_prop * 4 - 3]);
                }
                R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                if (L3_press_orig == 0 and press == 1) {
                    isrSidTran(sid_tran_list[current_page_prop * 4 - 2]);
                }
                L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R3-press");
                if (R3_press_orig == 0 and press == 1) {
                    isrSidTran(sid_tran_list[current_page_prop * 4 - 1]);
                }
                R3_press_orig = getprop("/instrumentation/fmz/fmz-key-R3-press");

                if (sel_sid_tran == sid_tran_list[current_page_prop * 4 - 4]) {
                    line4Left.setText(sid_tran_list[current_page_prop * 4 - 4] ~ " [SEL]");
                } else {
                    line4Left.setText(sid_tran_list[current_page_prop * 4 - 4]);
                }
                line4Left.setColor(0,255,0,255);
                if (sel_sid_tran == sid_tran_list[current_page_prop * 4 - 2]) {
                    line6Left.setText(sid_tran_list[current_page_prop * 4 - 2] ~ " [SEL]");
                } else {
                    line6Left.setText(sid_tran_list[current_page_prop * 4 - 2]);
                }
                line6Left.setColor(0,255,0,255);
                if (sel_sid_tran == sid_tran_list[current_page_prop * 4 - 3]){
                    line4Right.setText("[SEL] " ~ sid_tran_list[current_page_prop * 4 - 3]);
                } else {
                    line4Right.setText(sid_tran_list[current_page_prop * 4 - 3]);
                }               
                line4Right.setColor(0,255,0,255);
                if (sel_sid_tran == sid_tran_list[current_page_prop * 4 - 1]) {
                    line6Right.setText("[SEL] " ~ sid_tran_list[current_page_prop * 4 - 1]);
                } else {
                    line6Right.setText(sid_tran_list[current_page_prop * 4 - 1]);
                }
                line6Right.setColor(0,255,0,255);
            }

            if (current_page_prop < total_page_prop) {

                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1) {
                    isrSidTran(sid_tran_list[current_page_prop * 4 - 4]);
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
                if (R2_press_orig == 0 and press == 1) {
                    isrSidTran(sid_tran_list[current_page_prop * 4 - 3]);
                }
                R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                if (L3_press_orig == 0 and press == 1) {
                    isrSidTran(sid_tran_list[current_page_prop * 4 - 2]);
                }
                L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R3-press");
                if (R3_press_orig == 0 and press == 1) {
                    isrSidTran(sid_tran_list[current_page_prop * 4 - 1]);
                }
                R3_press_orig = getprop("/instrumentation/fmz/fmz-key-R3-press");
                if (sel_sid_tran == sid_tran_list[current_page_prop * 4 - 4]) {
                    line4Left.setText(sid_tran_list[current_page_prop * 4 - 4] ~ " [SEL]");
                } else {
                    line4Left.setText(sid_tran_list[current_page_prop * 4 - 4]);
                }
                line4Left.setColor(0,255,0,255);
                if (sel_sid_tran == sid_tran_list[current_page_prop * 4 - 2]) {
                    line6Left.setText(sid_tran_list[current_page_prop * 4 - 2] ~ " [SEL]");
                } else {
                    line6Left.setText(sid_tran_list[current_page_prop * 4 - 2]);
                }
                line6Left.setColor(0,255,0,255);
                if (sel_sid_tran == sid_tran_list[current_page_prop * 4 - 3]){
                    line4Right.setText("[SEL] " ~ sid_tran_list[current_page_prop * 4 - 3]);
                } else {
                    line4Right.setText(sid_tran_list[current_page_prop * 4 - 3]);
                }               
                line4Right.setColor(0,255,0,255);
                if (sel_sid_tran == sid_tran_list[current_page_prop * 4 - 1]) {
                    line6Right.setText("[SEL] " ~ sid_tran_list[current_page_prop * 4 - 1]);
                } else {
                    line6Right.setText(sid_tran_list[current_page_prop * 4 - 1]);
                }
                line6Right.setColor(0,255,0,255);
            }
            
            line3Left.setText("");            
            line5Left.setText("");            
            line7Left.setText("");
            line1Mid.setText("");
            line2Mid.setText("");
            line3Mid.setText("");
            line4Mid.setText("");
            line5Mid.setText("");
            line6Mid.setText("");
            line7Mid.setText("");
            line1Right.setText("");
            line2Right.setText("");
            line3Right.setText("");
            line5Right.setText("");
            line7Right.setText("ACTIVATE ▶");
            line7Right.setColor(255,255,255,255);
        }
    }
    fmz_setlistener(pageSidTran);

    var pagePerformanceInit = func (){
        if (page_title_text == "PERFORMANCE INIT") {
            if (current_page_prop == 1) {
                eachPageInit();
                perf_page_read[0] = 1;
                total_page_prop = 5;
                currentPage.setText(sprintf("%d", current_page_prop));
                currentPage.setColor(255,255,255,255);
                totalPage.setText(sprintf("%d",total_page_prop));
                totalPage.setColor(255,255,255,255);
                pageTitle.setText(sprintf("%s", "PERFORMANCE INIT"));
                pageTitle.setColor(255,255,255,255);
                pageTitleRight.setText(sprintf("%s", ""));
                var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
                if (R1_press_orig == 0 and press == 1 and scratch_pad_text != "") {
                    acft_tail_num = scratch_pad_text;
                    scratch_pad_text = "";
                } else if (R1_press_orig == 0 and press == 1 and scratch_pad_message == "DELETE") {
                    acft_tail_num = "";
                    scratch_pad_message = "";
                } else if (R1_press_orig == 0 and press == 1) {
                    scratch_pad_message = "INVALID INPUT";
                }
                R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");

                line1Left.setText(sprintf("%s", "ACFT TYPE"));
                line1Left.setColor(255,255,255,255);
                line2Left.setText("BD-700");
                line2Left.setColor(0,255,0,255);
                line3Left.setText(sprintf("%s", "PERF MODE"));
                line3Left.setColor(255,255,255,255);
                line4Left.setText(sprintf("%s", perf_mode));
                line4Left.setColor(0,255,0,255);
                line5Left.setText(sprintf("%s", ""));
                line6Left.setText(sprintf("%s", ""));
                line7Left.setText(sprintf("%s", ""));
                line1Mid.setText(sprintf("%s", ""));
                line2Mid.setText(sprintf("%s", ""));
                line3Mid.setText(sprintf("%s", ""));
                line4Mid.setText(sprintf("%s", ""));
                line5Mid.setText(sprintf("%s", ""));
                line6Mid.setText(sprintf("%s", ""));
                line7Mid.setText(sprintf("%s", ""));
                line1Right.setText(sprintf("%s", "TAIL #"));
                line1Right.setColor(255,255,255,255);
                if (acft_tail_num == "") {
                    line2Right.setText(sprintf("%s", "------"));
                    line2Right.setColor(0,174,255,255);
                } else {
                    line2Right.setText(sprintf("%s", acft_tail_num));
                    line2Right.setColor(0,174,255,255);
                }
                line3Right.setText(sprintf("%s", ""));
                line4Right.setText(sprintf("%s", "OR ▶"));
                line4Right.setColor(255,255,255,255);
                line5Right.setText(sprintf("%s", ""));
                line6Right.setText(sprintf("%s", ""));
                line7Right.setText(sprintf("%s", ""));
            }
            if (current_page_prop == 2) {
                eachPageInit();
                perf_page_read[1] = 1;
                total_page_prop = 5;
                currentPage.setText(sprintf("%d", current_page_prop));
                currentPage.setColor(255,255,255,255);
                totalPage.setText(sprintf("%d",total_page_prop));
                totalPage.setColor(255,255,255,255);
                pageTitle.setText(sprintf("%s", "PERFORMANCE INIT"));
                pageTitle.setColor(255,255,255,255);
                pageTitleRight.setText(sprintf("%s", ""));

                var clb_spd_text = sprintf("%3d", clb_ias) ~ "/." ~ sprintf("%2d", (clb_mach*100)) ~ "M";
                var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                if (L1_press_orig == 0 and press == 1 and scratch_pad_message == "DELETE") {
                    clb_ias = 300;
                    clb_mach = 0.80;
                    scratch_pad_message = "";
                } else if (L1_press_orig == 0 and press == 1) {
                    scratch_pad_message = "INVALID INPUT";
                }
                L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1 and scratch_pad_message == "DELETE") {
                    crs_perf_type = "LRC";
                    scratch_pad_message = "";
                } else if (L2_press_orig == 0 and press == 1) {
                    scratch_pad_message = "INVALID INPUT";
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                if (L3_press_orig == 0 and press == 1 and scratch_pad_message == "DELETE") {
                    des_ias = 300;
                    des_mach = 0.80;
                    des_path_deg = 3.0;
                    scratch_pad_message = "";
                } else if (L3_press_orig == 0 and press == 1) {
                    scratch_pad_message = "INVALID INPUT";
                }
                L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L4-press");
                if (L4_press_orig == 0 and press == 1) {
                    page_title_text = "DEP/APP SPD";
                    current_page_prop = 1;
                }
                L4_press_orig = getprop("/instrumentation/fmz/fmz-key-L4-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
                if (R1_press_orig == 0 and press == 1) {
                    page_title_text = "CLIMB MODES";
                    current_page_prop = 1;
                }
                R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
                if (R2_press_orig == 0 and press == 1) {
                    page_title_text = "CRUISE MODES";
                    current_page_prop = 1;
                }
                R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R3-press");
                if (R3_press_orig == 0 and press == 1) {
                    page_title_text = "DESCENT MODES";
                    current_page_prop = 1;
                }
                R3_press_orig = getprop("/instrumentation/fmz/fmz-key-R3-press");

                line1Left.setText(sprintf("%s", "CLIMB"));
                line1Left.setColor(255,255,255,255);
                line2Left.setText(sprintf("%3d", clb_ias) ~ "/." ~ sprintf("%2d", (clb_mach*100)) ~ "ᴍ");
                line2Left.setColor(0,174,255,255);
                line3Left.setText(sprintf("%s", "CRUISE"));
                line3Left.setColor(255,255,255,255);
                if (crs_perf_type == "MANUAL") {
                    line4Left.setText(sprintf("%3d", crs_ias) ~ "/." ~ sprintf("%2d", (crs_mach*100)) ~ "ᴍ");
                } else {
                    line4Left.setText(sprintf("%s", crs_perf_type));  
                }
                line4Left.setColor(0,174,255,255);
                line5Left.setText(sprintf("%s", "DESCENT"));
                line5Left.setColor(255,255,255,255);
                if (des_ias == 340 and des_mach == 0.86 and des_path_deg == 4.0) {
                    line6Left.setText(sprintf("%s", "VMO/MMO/4.0°"));
                } else { 
                    line6Left.setText(sprintf("%3d", des_ias) ~ "/." ~ sprintf("%2d", (des_mach*100)) ~ "ᴍ/" ~ sprintf("%1.1f", des_path_deg) ~ "°");
                }
                line6Left.setColor(0,174,255,255);
                line7Left.setText(sprintf("%s", "◀ DEP/APP SPD"));
                line7Left.setColor(255,255,255,255);
                line1Mid.setText(sprintf("%s", ""));
                line2Mid.setText(sprintf("%s", ""));
                line3Mid.setText(sprintf("%s", ""));
                line4Mid.setText(sprintf("%s", ""));
                line5Mid.setText(sprintf("%s", ""));
                line6Mid.setText(sprintf("%s", ""));
                line7Mid.setText(sprintf("%s", ""));
                line1Right.setText(sprintf("%s", ""));
                line2Right.setText(sprintf("%s", "OR ▶"));
                line2Right.setColor(255,255,255,255);
                line3Right.setText(sprintf("%s", ""));
                line4Right.setText(sprintf("%s", "OR ▶"));
                line4Right.setColor(255,255,255,255);
                line5Right.setText(sprintf("%s", ""));
                line6Right.setText(sprintf("%s", "OR ▶"));
                line6Right.setColor(255,255,255,255);
                line7Right.setText(sprintf("%s", ""));
            }

            if (current_page_prop == 3) {
                eachPageInit();
                perf_page_read[2] = 1;
                total_page_prop = 5;
                currentPage.setText(sprintf("%d", current_page_prop));
                currentPage.setColor(255,255,255,255);
                totalPage.setText(sprintf("%d",total_page_prop));
                totalPage.setColor(255,255,255,255);
                pageTitle.setText("PERFORMANCE INIT-LB");
                pageTitle.setColor(255,255,255,255);
                pageTitleRight.setText("");
                line1Left.setText("STEP INCREMENT");
                line1Left.setColor(255,255,255,255);
                line1Mid.setText("");
                line1Right.setText("");
                line2Left.setText(sprintf("%d", step_incm));
                line2Left.setColor(0,174,255,255);
                line2Mid.setText("");
                line2Right.setText("");
                line3Left.setText("FUEL RESERVE");
                line3Left.setColor(255,255,255,255);
                line3Mid.setText("");
                line3Right.setText("");
                line4Left.setText(sprintf("%s", fuel_rsv_mode));
                line4Left.setColor(0,174,255,255);
                line4Mid.setText("");
                line4Right.setText("OR ▶");
                line4Right.setColor(255,255,255,255);
                line5Left.setText("TO/LDG FUEL");
                line5Left.setColor(255,255,255,255);
                line5Mid.setText("");
                line5Right.setText("");
                var to_ldg_fuel_text = sprintf("%03d", to_fuel) ~ "/" ~ sprintf("%03d", ldg_fuel) ~ " ʟʙ";
                line6Left.setText(to_ldg_fuel_text);
                line6Left.setColor(0,174,255,255);
                line6Mid.setText("");
                line6Right.setText("");
                line7Left.setText("");
                line7Mid.setText("");
                line7Right.setText("");

                var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                if (L1_press_orig == 0 and press == 1) {
                    if(num(scratch_pad_text) != nil){
                        if (num(scratch_pad_text) >= 0 and num(scratch_pad_text) <= 20) {
                            step_incm = num(scratch_pad_text);
                            scratch_pad_text = "";
                        }
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                }
                L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
                if (R2_press_orig == 0 and press == 1) {
                    page_title_text = "FUEL RESERVE";
                    current_page_prop = 1;
                }
                R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");
            }
            
            if (current_page_prop == 4) {
                eachPageInit();
                perf_page_read[3] = 1;
                total_page_prop = 5;
                currentPage.setText(sprintf("%d", current_page_prop));
                currentPage.setColor(255,255,255,255);
                totalPage.setText(sprintf("%d",total_page_prop));
                totalPage.setColor(255,255,255,255);
                pageTitle.setText("PERFORMANCE INIT");
                pageTitle.setColor(255,255,255,255);
                pageTitleRight.setText("");
                line1Left.setText("TRANS ALT");
                line1Left.setColor(255,255,255,255);
                line1Mid.setText("");
                line1Right.setText("SPD/ALT LIM");
                line1Right.setColor(255,255,255,255);
                line2Left.setText(sprintf("%d", trans_alt));
                line2Left.setColor(0,174,255,255);
                line2Mid.setText("");
                line2Right.setText(sprintf("%d", spd_alt_lim_s) ~ "/" ~ sprintf("%d", spd_alt_lim_a));
                line2Right.setColor(0,174,255,255);
                line3Left.setText("INIT CRZ ALT");
                line3Left.setColor(255,255,255,255);
                line3Mid.setText("");
                line3Right.setText("ISA DEV");
                line3Right.setColor(255,255,255,255);
                
                if (init_crz_alt_mod == "OPTIMUM") {
                    if (init_crz_alt < 0) {
                        line4Left.setText("OPTIMUM");
                        line4Left.setFontSize(48, 1);
                    } else {
                        var init_fl = init_crz_alt / 100;
                        line4Left.setText("FL" ~ sprintf("%03d", init_fl) ~ " (OPTIMUM)");
                        line4Left.setFontSize(36, 1);
                    }
                } else if (init_crz_alt_mod == "MANUAL") {
                    var init_fl = init_crz_alt / 100;
                    line4Left.setText("FL" ~ sprintf("%03d", init_fl));
                    line4Left.setFontSize(48, 1);
                } else {
                    line4Left.setText(init_crz_alt_mod);
                }
                
                line4Left.setColor(0,174,255,255);
                line4Mid.setText("");
                if (isa_dev_c < 0) {
                   line4Right.setText(sprintf("%d", isa_dev_c) ~ "°C");
                } else {
                   line4Right.setText("+" ~ sprintf("%d", isa_dev_c) ~ "°C");
                }
                
                line4Right.setColor(0,174,255,255);
                line5Left.setText("CRZ WINDS");
                line5Left.setColor(255,255,255,255);
                line5Mid.setText("");
                line5Right.setText("AT ALTITUDE");
                line5Right.setColor(255,255,255,255);
                line6Left.setText(sprintf("%d", crz_wind_d) ~ "ᴛ" ~ "/" ~ sprintf("%d", crz_wind_s));
                line6Left.setColor(0,174,255,255);
                line6Mid.setText("");
                if (init_crz_alt > 0) {
                    crz_wind_at_alt = init_crz_alt;
                } else {
                    crz_wind_at_alt = 40000;
                }
                var crz_wind_at_fl = crz_wind_at_alt / 100;
                line6Right.setText("FL" ~ sprintf("%03d", crz_wind_at_fl));
                line6Right.setColor(0,174,255,255);
                line7Left.setText("◀ PERF PLAN");
                line7Left.setColor(255,255,255,255);
                line7Mid.setText("");
                line7Right.setText("");

                var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                if (L1_press_orig == 0 and press == 1) {
                    var trans_alt_inp = num(scratch_pad_text) or -9999;
                    if (trans_alt_inp >= 0 and trans_alt_inp < 99999) {
                        trans_alt = trans_alt_inp;
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");
                var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
                if (R1_press_orig == 0 and press == 1) {
                    var sal_inp = scratch_pad_text;
                    var sal_slash_pos = find("/", sal_inp);
                    var grab_spd_lim = substr(sal_inp, 0, sal_slash_pos);
                    grab_spd_lim = num(grab_spd_lim);
                    var grab_lim_alt = substr(sal_inp, (sal_slash_pos + 1));
                    grab_lim_alt = num(grab_lim_alt);
                    if (sal_slash_pos == 3 and grab_lim_alt != nil and grab_spd_lim != nil) {
                        spd_alt_lim_s = grab_spd_lim;
                        spd_alt_lim_a = grab_lim_alt;
                    } else if (sal_slash_pos == 0 and grab_lim_alt != nil) {
                        spd_alt_lim_a = grab_lim_alt;
                    } else if (sal_slash_pos == 3 and size(sal_inp) == 4 and grab_spd_lim != nil) {
                        spd_alt_lim_s = grab_spd_lim;
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");
                
                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1) {
                    var init_alt_inp = scratch_pad_text;
                    var grab_fl_mark = substr(init_alt_inp, 0, 2);
                    var num_init_alt = num(init_alt_inp) or -9999;
                    if (grab_fl_mark == "FL") {
                        var grab_fl = substr(init_alt_inp, 2, 3);
                        grab_fl = num(grab_fl);
                        init_crz_alt = grab_fl * 100;
                        init_crz_alt_mod = "MANUAL";
                    } else if (num_init_alt >= 0) {
                        init_crz_alt = init_alt_inp;                    
                        init_crz_alt_mod = "MANUAL";
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
                if (R2_press_orig == 0 and press == 1) {
                    var isa_dev_inp = num(scratch_pad_text) or -9999;
                    if (abs(isa_dev_inp) < 100) {
                        isa_dev_c = isa_dev_inp;
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                if (L3_press_orig == 0 and press == 1) {
                    var crz_wind_inp = scratch_pad_text;
                    var wind_dir_inp = substr(crz_wind_inp, 0, 3);
                    wind_dir_inp = num(wind_dir_inp) or -9999;
                    var dir_inp_vld = (wind_dir_inp >=0 and wind_dir_inp <= 359);
                    var wind_slh_pos = find("/", crz_wind_inp);
                    var wind_spd_inp = substr(crz_wind_inp, wind_slh_pos + 1);
                    wind_spd_inp = num(wind_spd_inp) or -9999;
                    var spd_inp_vld = (wind_spd_inp >=0);
                    var tur_pos = find("T", crz_wind_inp);
                    var mag_pos = find("M", crz_wind_inp);
                    if (wind_slh_pos == 4 and tur_pos == 3) {
                        wind_slh_pos = 3;
                    } else if (wind_slh_pos == 4 and mag_pos == 3) {
                        var mag_var = getprop("/environment/magnetic-variation-deg") or 0;
                        wind_slh_pos = 3;
                        wind_dir_inp = wind_dir_inp + mag_var;
                    }
                    wind_dir_inp = math.fmod(wind_dir_inp, 360);
                    if (wind_dir_inp < 0) {
                        wind_dir_inp = wind_dir_inp + 360;
                    }
                    if (wind_slh_pos == 3 and dir_inp_vld and spd_inp_vld) {
                        crz_wind_d = wind_dir_inp;
                        crz_wind_s = wind_spd_inp;
                    } else if (wind_slh_pos == 3 and dir_inp_vld) {
                        crz_wind_d = wind_dir_inp;
                    } else if (wind_slh_pos == 0 and spd_inp_vld) {
                        crz_wind_s = wind_spd_inp;

                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R3-press");
                if (R3_press_orig == 0 and press == 1) {
                    var wind_at_alt_inp = scratch_pad_text;
                    var grab_fl_mark = substr(wind_at_alt_inp, 0, 2);
                    var num_wind_at_alt = num(wind_at_alt_inp) or -9999;
                    if (grab_fl_mark == "FL") {
                        var grab_fl = substr(wind_at_alt_inp, 2, 3);
                        grab_fl = num(grab_fl);
                        crz_wind_at_alt = grab_fl * 100;
                    } else if (num_init_alt >= 0) {
                        crz_wind_at_alt = init_alt_inp;                    
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                R3_press_orig = getprop("/instrumentation/fmz/fmz-key-R3-press");
            }
            
            if (current_page_prop == 5) {
                eachPageInit();
                perf_page_read[4] = 1;
                total_page_prop = 5;
                currentPage.setText(sprintf("%d", current_page_prop));
                currentPage.setColor(255,255,255,255);
                totalPage.setText(sprintf("%d",total_page_prop));
                totalPage.setColor(255,255,255,255);
                pageTitle.setText("PERFORMANCE INIT-LB");
                pageTitle.setColor(255,255,255,255);
                pageTitleRight.setText("");
                line1Left.setText("BOW");
                line1Mid.setText("");
                line1Right.setText("PASS/@ LB");
                line2Left.setText(sprintf("%d", bsc_oprt_wght));
                line2Left.setColor(0,174,255,255);
                line2Mid.setText("");
                if (pax_number <= 30 and weight_per_pax >= 0) {
                    line2Right.setText(sprintf("%d", pax_number) ~ "/" ~ sprintf("%d", weight_per_pax));
                } else {
                    line2Right.setText("--" ~ "/" ~ "---");
                }
                line2Right.setColor(0,174,255,255);
                line3Left.setText(" FUEL (GAUGE)");
                line3Mid.setText("");
                line3Right.setText("PASS WT");
                var gauge_fuel_lbs = getprop("/consumables/fuel/total-fuel-lbs");
                line4Left.setText(sprintf("%d", total_fuel_lbs_inp) ~ " (" ~ sprintf("%d", gauge_fuel_lbs) ~ ")");
                    
                if (fuel_qnt_snsr == 1) {
                    line4Left.setFontSize(36, 1);
                } else {
                    line4Left.setFontSize(48, 1);
                }
                
                line4Left.setColor(0,174,255,255);
                line4Mid.setText("");
                
                var ttl_pax_wght = pax_number * weight_per_pax;
                if (ttl_pax_wght >= 0) {
                    line4Right.setText(sprintf("%d", ttl_pax_wght));
                } else {
                    line4Right.setText("-----");
                }

                line4Right.setColor(0,174,255,255);
                line5Left.setText("CARGO");
                line5Mid.setText("");
                line5Right.setText("GROSS WT");
                
                if (cargo_weight < 0) {
                    line6Left.setText("---");
                } else {
                    line6Left.setText(sprintf("%d", cargo_weight));
                }
                
                line6Left.setColor(0,174,255,255);
                line6Mid.setText("");
                
                if (bsc_oprt_wght >= 0 and cargo_weight >= 0 and ttl_pax_wght >= 0 and total_fuel_lbs_inp >= 0) {
                    var gross_weight = bsc_oprt_wght + cargo_weight + ttl_pax_wght + total_fuel_lbs_inp;
                } else {
                    var gross_weight = -99999;
                }
                if (gross_weight > 0) {
                    line6Right.setText(sprintf("%d", gross_weight));
                } else {
                    line6Right.setText("-----");
                }
                line6Right.setColor(0,174,255,255);
                line7Left.setText("");
                line7Mid.setText("");
                if (checkPerfInit() == "can confirm"){
                    line7Right.setText("CONFIRM INIT ▶");
                    l7rRectangle.setVisible(1);
                } else {
                    line7Right.setText("");
                    l7rRectangle.setVisible(0);                    
                }
                
                var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                if (L1_press_orig == 0 and press == 1) {
                    var bow_inp = num(scratch_pad_text) or -9999;
                    if (bow_inp >= 0 and bow_inp < 98250) {
                        bsc_oprt_wght = bow_inp;
                    } else if (scratch_pad_message == "DELETE") {
                        bsc_oprt_wght = 45000;
                        scratch_pad_message = "";
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1) {
                    var fuel_wght_pad = num(scratch_pad_text) or -9999;
                    if (fuel_wght_pad >= 0 and fuel_wght_pad <= 46586) {
                        fuel_qnt_snsr = 0;
                        total_fuel_lbs_inp = fuel_wght_pad;
                    } else if (scratch_pad_message == "DELETE") {
                        fuel_qnt_snsr = 1;
                        total_fuel_lbs_inp = getprop("/consumables/fuel/total-fuel-lbs");
                        scratch_pad_message = "";
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
                
                var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                if (L3_press_orig == 0 and press == 1) {
                    var cargo_wght_pad = num(scratch_pad_text) or -9999;
                    if (cargo_wght_pad >= 0) {
                        cargo_weight = cargo_wght_pad;
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
                if (R1_press_orig == 0 and press == 1) {
                    var pax_data_pad = scratch_pad_text;
                    var pax_slsh_pos = find("/", pax_data_pad);
                    var pax_data_size = size(pax_data_pad);
                    if (pax_slsh_pos == 0) {
                        var sub_wght_per_pax = substr(pax_data_pad, (pax_slsh_pos + 1));
                        sub_wght_per_pax = num(sub_wght_per_pax) or -9999;
                        if (sub_wght_per_pax >= 0) {
                            weight_per_pax = sub_wght_per_pax;
                        } else {
                            scratch_pad_message = "INVALID INPUT";
                        }
                    } else if (pax_slsh_pos > 0 and pax_slsh_pos == (pax_data_size - 1)) {
                        var sub_pax_num = substr(pax_data_pad, 0, pax_slsh_pos);
                        sub_pax_num = num(sub_pax_num) or -9999;
                        if (sub_pax_num >= 0 and sub_pax_num <= 30) {
                            pax_number = sub_pax_num;
                        } else {
                            scratch_pad_message = "INVALID INPUT";
                        }
                    } else if (pax_slsh_pos > 0) {
                        var sub_pax_num = substr(pax_data_pad, 0, pax_slsh_pos);
                        sub_pax_num = num(sub_pax_num) or -9999;
                        var sub_wght_per_pax = substr(pax_data_pad, (pax_slsh_pos + 1));
                        sub_wght_per_pax = num(sub_wght_per_pax) or -9999;
                        if (sub_wght_per_pax >= 0 and sub_pax_num >= 0 and sub_pax_num <= 30) {
                            pax_number = sub_pax_num;
                            weight_per_pax = sub_wght_per_pax;
                        } else {
                            scratch_pad_message = "INVALID INPUT";
                        }
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");
                var press = getprop("/instrumentation/fmz/fmz-key-R4-press");
                if (R4_press_orig == 0 and press == 1) {
                    if (init_crz_alt_mod == "OPTIMUM" and checkPerfInit() == "can confirm") {
                        bsc_oprt_wght1 = bsc_oprt_wght;
                        cargo_weight1 = cargo_weight;
                        pax_number1 = pax_number;
                        weight_per_pax1 = weight_per_pax;
                        init_crz_alt_mod1 = init_crz_alt_mod;
                        fuel_opm_alt_prd();
                        init_crz_alt = opm_init_alt;
                        init_crz_alt1 = init_crz_alt;
                        crn_crz_alt = init_crz_alt;
                        page_title_text = "PERF DATA";
                        current_page_prop = 1;
                    } else if (checkPerfInit() == "can confirm") {
                        bsc_oprt_wght1 = bsc_oprt_wght;
                        cargo_weight1 = cargo_weight;
                        pax_number1 = pax_number;
                        weight_per_pax1 = weight_per_pax;
                        init_crz_alt_mod1 = init_crz_alt_mod;
                        init_crz_alt1 = init_crz_alt;
                        fuel_opm_alt_prd();
                        crn_crz_alt = init_crz_alt;
                        page_title_text = "PERF DATA";
                        current_page_prop = 1;
                    } else if (checkPerfInit() == "incompleted") {
                        scratch_pad_message = "INCOMPLETED INPUT";
                    } else if (checkPerfInit() == "don't need") {
                        page_title_text = "PERF DATA";
                        current_page_prop = 1;
                    }
                }
                R4_press_orig = getprop("/instrumentation/fmz/fmz-key-R4-press");
            }
        }
    }
    fmz_setlistener(pagePerformanceInit);

    var pageClimbModes = func(){
        if (page_title_text == "CLIMB MODES") {
            eachPageInit();
            total_page_prop = 1;
            pageTitle.setText(sprintf("%s", "CLIMB MODES"));
            pageTitleRight.setText(sprintf("%s", ""));
            currentPage.setText(sprintf("%d", current_page_prop));
            totalPage.setText(sprintf("%d", total_page_prop));
            line1Left.setText(sprintf("%s", "MANUAL"));
            line1Left.setColor(255,255,255,255);
            if (clb_ias == 300 and clb_mach == 0.80) {
                line2Left.setText(sprintf("%3d", clb_ias) ~ "/." ~ sprintf("%2d", (clb_mach*100)) ~ "M");
                line2Left.setFontSize(36, 1);
            } else {
                line2Left.setText(sprintf("%3d", clb_ias) ~ "/." ~ sprintf("%2d", (clb_mach*100)) ~ "ᴍ" ~ " [ACT]");
                line2Left.setFontSize(48, 1);
            }
            line2Left.setColor(0,174,255,255);
            line3Left.setText(sprintf("%s", ""));
            if (clb_ias == 300 and clb_mach == 0.80) {
                line4Left.setText(sprintf("%s", "◀ 300/.80ᴍ [ACT]"));
                line4Left.setFontSize(48, 1);
            } else {
                line4Left.setText(sprintf("%s", "◀ 300/.80M"));
                line4Left.setFontSize(36, 1);
            }
            line4Left.setColor(0,174,255,255);
            line5Left.setText(sprintf("%s", ""));
            line6Left.setText(sprintf("%s", ""));
            line7Left.setText(sprintf("%s", ""));
            line1Mid.setText(sprintf("%s", ""));
            line2Mid.setText(sprintf("%s", ""));
            line3Mid.setText(sprintf("%s", ""));
            line4Mid.setText(sprintf("%s", ""));
            line5Mid.setText(sprintf("%s", ""));
            line6Mid.setText(sprintf("%s", ""));
            line7Mid.setText(sprintf("%s", ""));
            line1Right.setText(sprintf("%s", "RETURN ▶"));
            line1Right.setColor(255,255,255,255);
            line2Right.setText(sprintf("%s", ""));
            line3Right.setText(sprintf("%s", ""));
            line4Right.setText(sprintf("%s", ""));
            line5Right.setText(sprintf("%s", ""));
            line6Right.setText(sprintf("%s", ""));
            line7Right.setText(sprintf("%s", ""));

            var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
            if (L1_press_orig == 0 and press == 1) {
                var slash_pos = find("/", scratch_pad_text);
                if (slash_pos == -1) {
                    scratch_pad_message = "INVALID INPUT";
                } else if (num(substr(scratch_pad_text, 0, slash_pos))==nil){
                    if (num(substr(scratch_pad_text, slash_pos+1))==nil) {
                        scratch_pad_message = "INVALID INPUT";
                    }
                } else if (num(substr(scratch_pad_text, 0, slash_pos))!=nil){
                    if (num(substr(scratch_pad_text, 0, slash_pos))>340) {
                        scratch_pad_message = "INVALID INPUT";
                    }
                } else if (num(substr(scratch_pad_text, slash_pos+1))!=nil) {
                    if (num(substr(scratch_pad_text, slash_pos+1))>=0.90) {
                        scratch_pad_message = "INVALID INPUT";
                    }
                }
                if (scratch_pad_message != "INVALID INPUT") {
                    if (num(substr(scratch_pad_text, 0, slash_pos))!=nil) {
                        clb_ias = num(substr(scratch_pad_text, 0, slash_pos));
                    } 
                    if (num(substr(scratch_pad_text, slash_pos+1))!=nil) {
                        clb_mach = num(substr(scratch_pad_text, slash_pos+1));
                    }
                    line2Left.setFontSize(48, 1);
                    line4Left.setFontSize(48, 1);
                    page_title_text = "PERFORMANCE INIT";
                    current_page_prop = 2;
                    scratch_pad_text = "";
                }
            }
            L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");

            var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
            if (L2_press_orig == 0 and press == 1) {
                clb_ias = 300;
                clb_mach = 0.8;
                line2Left.setFontSize(48, 1);
                line4Left.setFontSize(48, 1);
                page_title_text = "PERFORMANCE INIT";
                current_page_prop = 2;
            }
            L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

            var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
            if (R1_press_orig == 0 and press == 1) {
                line2Left.setFontSize(48, 1);
                line4Left.setFontSize(48, 1);
                page_title_text = "PERFORMANCE INIT";
                current_page_prop = 2;
            }
            R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");
        }
    }
    fmz_setlistener(pageClimbModes);

    var pageCruiseModes = func() {
        if (page_title_text == "CRUISE MODES") {
            eachPageInit();
            total_page_prop = 1;
            pageTitle.setText(sprintf("%s", "CRUISE MODES"));
            pageTitleRight.setText("");
            currentPage.setText(sprintf("%d", current_page_prop));
            totalPage.setText(sprintf("%d", total_page_prop));
            line1Left.setText("MANUAL");
            line1Left.setColor(255,255,255,255);
            line1Mid.setText("");
            line1Right.setText("RETURN ▶");
            line1Right.setColor(255,255,255,255);
            if (crs_perf_type == "MANUAL") {
                line2Left.setText(sprintf("%3d", crs_ias) ~ "/." ~ sprintf("%2d", (crs_mach*100)) ~ "ᴍ" ~ " [ACT]");
                line2Left.setFontSize(48, 1);
            } else {
                line2Left.setText(sprintf("%3d", crs_ias) ~ "/." ~ sprintf("%2d", (crs_mach*100)) ~ "M");
                line2Left.setFontSize(36, 1);
            }
            line2Left.setColor(0,174,255,255);
            line2Mid.setText("");
            line2Right.setText("");
            line3Left.setText("");
            line3Mid.setText("");
            line3Right.setText("");
            if (crs_perf_type == "LRC") {
                line4Left.setText("◀ LRC [ACT]");
                line4Left.setFontSize(48,1);
            } else { 
                line4Left.setText("◀ LRC");
                line4Left.setFontSize(36,1);
            }
            line4Left.setColor(0,174,255,255);
            line4Mid.setText("");
            if (crs_perf_type == "MAX END") {
                line4Right.setText("[ACT] MAX END ▶");
                line4Right.setFontSize(48,1);
            } else {
                line4Right.setText("MAX END ▶");
                line4Right.setFontSize(36,1);
            }
            line4Right.setColor(0,174,255,255);
            line5Left.setText("");
            line5Mid.setText("");
            line5Right.setText("");
            if (crs_perf_type == "MAX SPD") {
                line6Left.setText("◀ MAX SPD [ACT]");
                line6Left.setFontSize(48,1);
            } else {
                line6Left.setText("◀ MAX SPD");
                line6Left.setFontSize(36,1);
            }
            line6Left.setColor(0,174,255,255);
            line6Mid.setText("");
            line6Right.setText("");
            line7Left.setText("");
            line7Mid.setText("");
            line7Right.setText("");

            var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
            if (L1_press_orig == 0 and press == 1) {
                var slash_pos = find("/", scratch_pad_text);
                if (slash_pos == -1) {
                    scratch_pad_message = "INVALID INPUT";
                } else if (num(substr(scratch_pad_text, 0, slash_pos))==nil){
                    if (num(substr(scratch_pad_text, slash_pos+1))==nil) {
                        scratch_pad_message = "INVALID INPUT";
                    }
                } else if (num(substr(scratch_pad_text, 0, slash_pos))!=nil){
                    if (num(substr(scratch_pad_text, 0, slash_pos))>340) {
                        scratch_pad_message = "INVALID INPUT";
                    }
                } else if (num(substr(scratch_pad_text, slash_pos+1))!=nil) {
                    if (num(substr(scratch_pad_text, slash_pos+1))>=0.90) {
                        scratch_pad_message = "INVALID INPUT";
                    }
                }
                if (scratch_pad_message != "INVALID INPUT") {
                    if (num(substr(scratch_pad_text, 0, slash_pos))!=nil) {
                        crs_ias = num(substr(scratch_pad_text, 0, slash_pos));
                    } 
                    if (num(substr(scratch_pad_text, slash_pos+1))!=nil) {
                        crs_mach = num(substr(scratch_pad_text, slash_pos+1));
                    }
                    crs_perf_type = "MANUAL";
                    line2Left.setFontSize(48, 1);
                    line4Left.setFontSize(48, 1);
                    line4Right.setFontSize(48, 1);
                    line6Left.setFontSize(48,1);
                    page_title_text = "PERFORMANCE INIT";
                    current_page_prop = 2;
                    scratch_pad_text = "";
                }
            }
            L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");

            var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
            if (L2_press_orig == 0 and press == 1) {
                crs_perf_type = "LRC";
                crs_ias = 300;
                crs_mach = 0.8;
                line2Left.setFontSize(48, 1);
                line4Left.setFontSize(48, 1);
                line4Right.setFontSize(48, 1);
                line6Left.setFontSize(48,1);
                page_title_text = "PERFORMANCE INIT";
                current_page_prop = 2;
            }
            L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

            var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
            if (L3_press_orig == 0 and press == 1) {
                crs_perf_type = "MAX SPD";
                crs_ias = 340;
                crs_mach = 0.86;
                line2Left.setFontSize(48, 1);
                line4Left.setFontSize(48, 1);
                line4Right.setFontSize(48, 1);
                line6Left.setFontSize(48,1);
                page_title_text = "PERFORMANCE INIT";
                current_page_prop = 2;
            }
            L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");

            var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
            if (R2_press_orig == 0 and press == 1) {
                crs_perf_type = "MAX END";
                crs_ias = 230;
                crs_mach = 0.62;
                line2Left.setFontSize(48, 1);
                line4Left.setFontSize(48, 1);
                line4Right.setFontSize(48, 1);
                line6Left.setFontSize(48,1);
                page_title_text = "PERFORMANCE INIT";
                current_page_prop = 2;
            }
            R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");

            var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
            if (R1_press_orig == 0 and press == 1) {
                line2Left.setFontSize(48, 1);
                line4Left.setFontSize(48, 1);
                line4Right.setFontSize(48, 1);
                line6Left.setFontSize(48,1);
                page_title_text = "PERFORMANCE INIT";
                current_page_prop = 2;
            }
            R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");
        }
    }
    fmz_setlistener(pageCruiseModes);

    var pageDescentModes = func() {
        if (page_title_text == "DESCENT MODES") {
            eachPageInit();
            total_page_prop = 1;
            pageTitle.setText(sprintf("%s", "DESCENT MODES"));
            pageTitleRight.setText("");
            currentPage.setText(sprintf("%d", current_page_prop));
            totalPage.setText(sprintf("%d", total_page_prop));
            line1Left.setText("MANUAL");
            line1Left.setColor(255,255,255,255);
            line1Mid.setText("");
            line1Right.setText("RETURN ▶");
            line1Right.setColor(255,255,255,255);
            line2Left.setColor(0,174,255,255);
            line2Mid.setText("");
            line2Right.setText("");
            line3Left.setText("");
            line3Mid.setText("");
            line3Right.setText("");
            if (des_ias == 300 and des_mach == 0.8 and des_path_deg == 3.0) {
                line4Left.setText("◀ 300/.80ᴍ/3.0° [ACT]");
                line4Left.setFontSize(48,1);

                line2Left.setText(sprintf("%3d", des_ias) ~ "/." ~ sprintf("%2d", (des_mach*100)) ~ "M" ~ "/" ~ sprintf("%3.1f", des_path_deg) ~"°");
                line2Left.setFontSize(36, 1);
                line6Left.setText("◀ VMO/MMO/4.0°");
                line6Left.setFontSize(36,1);
            } else if (des_ias == 340 and des_mach == 0.86 and des_path_deg == 4.0) {
                line6Left.setText("◀ VMO/MMO/4.0° [ACT]");
                line6Left.setFontSize(48,1);

                line4Left.setText("◀ 300/.80M/3.0°");
                line4Left.setFontSize(36,1);
                line2Left.setText(sprintf("%3d", des_ias) ~ "/." ~ sprintf("%2d", (des_mach*100)) ~ "M" ~ "/" ~ sprintf("%3.1f", des_path_deg) ~"°");
                line2Left.setFontSize(36, 1);
            } else { 
                line2Left.setText(sprintf("%3d", des_ias) ~ "/." ~ sprintf("%2d", (des_mach*100)) ~ "ᴍ" ~ "/" ~ sprintf("%3.1f", des_path_deg) ~"°" ~ " [ACT]");
                line2Left.setFontSize(48, 1);

                line4Left.setText("◀ 300/.80M/3.0°");
                line4Left.setFontSize(36,1);
                line6Left.setText("◀ VMO/MMO/4.0°");
                line6Left.setFontSize(36,1);
            }
            line4Left.setColor(0,174,255,255);
            line4Mid.setText("");
            line4Right.setText("");
            line5Left.setText("");
            line5Mid.setText("");
            line5Right.setText("");
            line6Left.setColor(0,174,255,255);
            line6Mid.setText("");
            line6Right.setText("");
            line7Left.setText("");
            line7Mid.setText("");
            line7Right.setText("");

            var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
            if (L1_press_orig == 0 and press == 1) {
                var des_ias_inp = num(substr(scratch_pad_text, 0, 3)) or 9999;
                var slash_1 = find("/", scratch_pad_text);
                var rmn_pad_text = substr(scratch_pad_text, slash_1 + 1);
                var slash_2 = find("/", rmn_pad_text);
                if (slash_2 == -1) {
                    slash_2 = 9999;
                }
                var des_mach_inp = num(substr(rmn_pad_text, 0, slash_2)) or 9999;
                var des_deg_inp = num(substr(rmn_pad_text, slash_2 + 1)) or 9999;
                var des_inp_vld = 0;

                if (des_ias_inp >= 108 and des_ias_inp <= 340) {
                    des_ias = des_ias_inp;
                    des_inp_vld = 1;
                } 
                if (des_mach_inp >= 0.6 and des_mach_inp <= 0.90) {
                    des_mach = des_mach_inp;
                    des_inp_vld = 1;
                } 
                if (des_deg_inp > 0 and des_deg_inp < 10.0){
                    des_path_deg = des_deg_inp;
                    des_inp_vld = 1;
                }
                
                if (des_inp_vld == 0) {
                    scratch_pad_message = "INVALID INPUT";
                }

                if (scratch_pad_message != "INVALID INPUT") {
                    scratch_pad_text = "";
                    line2Left.setFontSize(48, 1);
                    line4Left.setFontSize(48, 1);
                    line4Right.setFontSize(48, 1);
                    line6Left.setFontSize(48,1);
                    page_title_text = "PERFORMANCE INIT";
                    current_page_prop = 2;
                }
            }
            L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");

            var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
            if (L2_press_orig == 0 and press == 1) {
                des_ias = 300;
                des_mach = 0.8;
                des_path_deg = 3.0;
                line2Left.setFontSize(48, 1);
                line4Left.setFontSize(48, 1);
                line4Right.setFontSize(48, 1);
                line6Left.setFontSize(48,1);
                page_title_text = "PERFORMANCE INIT";
                current_page_prop = 2;
            }
            L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

            var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
            if (L3_press_orig == 0 and press == 1) {
                des_ias = 340;
                des_mach = 0.86;
                des_path_deg = 4.0;
                line2Left.setFontSize(48, 1);
                line4Left.setFontSize(48, 1);
                line4Right.setFontSize(48, 1);
                line6Left.setFontSize(48,1);
                page_title_text = "PERFORMANCE INIT";
                current_page_prop = 2;
            }
            L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");

            var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
            if (R1_press_orig == 0 and press == 1) {
                line2Left.setFontSize(48, 1);
                line4Left.setFontSize(48, 1);
                line4Right.setFontSize(48, 1);
                line6Left.setFontSize(48,1);
                page_title_text = "PERFORMANCE INIT";
                current_page_prop = 2;
            }
            R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");
        }
    }
    fmz_setlistener(pageDescentModes);

    var pageDepAppSpd = func() {
        if (page_title_text == "DEP/APP SPD") {
            eachPageInit();
            total_page_prop = 4;
            pageTitleRight.setText("");
            totalPage.setText(sprintf("%d", total_page_prop));
            currentPage.setText(sprintf("%d", current_page_prop));
            if (page_title_text == "DEP/APP SPD" and current_page_prop == 1) {
                eachPageInit();
                pageTitle.setText("DEPARTURE SPEED");
                pageTitle.setColor(255,255,255,255);
                line1Left.setText("SPEED LIMIT");
                line1Left.setColor(255,255,255,255);
                line1Mid.setText("");
                line1Right.setText("RETURN ▶");
                line2Left.setText(sprintf("%d", dep_spd_lmt));
                line2Left.setColor(0,174,255,255);
                line2Mid.setText("");
                line2Right.setText("");
                line3Left.setText("");
                line3Mid.setText("    AGL <------------------  LIMIT  ------------------> NM");
                line3Mid.setColor(255,255,255,255);
                line3Right.setText("");
                line4Left.setText(sprintf("%d", dep_agl_lmt));
                line4Left.setColor(0,174,255,255);
                line4Mid.setText("");
                line4Right.setText(sprintf("%.1f", dep_nm_lmt));
                line4Right.setColor(0,174,255,255);
                line5Left.setText("");
                line5Mid.setText("");
                line5Right.setText("");
                line6Left.setText("");
                line6Mid.setText("");
                line6Right.setText("");
                line7Left.setText("");
                line7Mid.setText("");
                line7Right.setText("");

                var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
                if (R1_press_orig == 0 and press == 1) {
                    page_title_text = "PERFORMANCE INIT";
                    current_page_prop = 2;
                }
                R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                if (L1_press_orig == 0 and press == 1) {
                    var dep_spd_lmt_inp = num(scratch_pad_text) or 9999;
                    if (dep_spd_lmt_inp <= (cln_vref / 1.3) or dep_spd_lmt_inp > 340) {
                        scratch_pad_message = "INVALID INPUT";
                    } else {
                        dep_spd_lmt = dep_spd_lmt_inp;
                    }
                    scratch_pad_text = "";
                }
                L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1) {
                    var dep_agl_lmt_inp = num(scratch_pad_text) or -9999;
                    if (dep_agl_lmt_inp >= 0 and dep_agl_lmt_inp < 10000) {
                        dep_agl_lmt = dep_agl_lmt_inp;
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
                if (R2_press_orig == 0 and press == 1) {
                    var dep_nm_lmt_inp = num(scratch_pad_text) or -9999;
                    if (dep_nm_lmt_inp >= 0 and dep_nm_lmt_inp < 999) {
                        dep_nm_lmt = dep_nm_lmt_inp;
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                R2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
            }

            if (page_title_text == "DEP/APP SPD" and current_page_prop == 2) {
                eachPageInit();
                page_prop_tmp = current_page_prop;
                pageTitle.setText("APPROACH SPEED");
                pageTitle.setColor(255,255,255,255);
                line1Left.setText("CLEAN");
                line1Left.setColor(255,255,255,255);
                line1Mid.setText("");
                line1Right.setText("RETURN ▶");
                line1Right.setColor(255,255,255,255);
                line2Left.setText(sprintf("%d", cln_app_lmt));
                line2Left.setColor(0,174,255,255);
                line2Mid.setText("");
                line2Right.setText("");
                line3Left.setText("DIST TO DESTINATION");
                line3Left.setColor(255,255,255,255);
                line3Mid.setText("");
                line3Right.setText("");
                line4Left.setText(sprintf("%.1f", app_init_dst) ~ "NM");
                line4Left.setColor(0,174,255,255);
                line4Mid.setText("");
                line4Right.setText("");
                line5Left.setText("        FIRST APP WPT");
                line5Left.setColor(255,255,255,255);
                line5Mid.setText("");
                line5Right.setText("");
                line6Left.setText("  " ~ app_1st_wpt);
                line6Left.setColor(0,255,0,255);
                line6Mid.setText("                                   OR");
                line6Mid.setColor(255,255,255,255);
                line6Mid.setFontSize(48,1);
                if (app_1st_wpt == "YES") {
                    line6Right.setText("NO ▶");
                } else {
                    line6Right.setText("YES ▶");
                }
                line6Right.setColor(255,255,255,255);
                line6Right.setFontSize(36,1);
                line7Left.setText("");
                line7Mid.setText("");
                line7Right.setText("");

                var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
                if (R1_press_orig == 0 and press == 1) {
                    page_title_text = "PERFORMANCE INIT";
                    current_page_prop = 2;
                    line6Mid.setFontSize(36,1);
                    line6Right.setFontSize(48,1);
                }
                R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R3-press");
                if (R3_press_orig == 0 and press == 1) {
                    if (app_1st_wpt == "YES") {
                        app_1st_wpt = "NO";
                    } else {
                        app_1st_wpt = "YES";
                    }
                }
                R3_press_orig = getprop("/instrumentation/fmz/fmz-key-R3-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                if (L1_press_orig == 0 and press == 1) {
                    var cln_app_lmt_inp = num(scratch_pad_text) or 9999;
                    if (cln_app_lmt_inp > cln_vso and cln_app_lmt_inp < 340) {
                        cln_app_lmt = cln_app_lmt_inp;
                        scratch_pad_text = "";
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                }
                L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");
                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1) {
                    var app_init_dst_inp = num(scratch_pad_text) or -9999;
                    if (app_init_dst_inp > 0) {
                        app_init_dst = app_init_dst_inp;
                        scratch_pad_text = "";
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");
            }
            if (page_prop_tmp == 2 and current_page_prop != 2) {
                line6Mid.setFontSize(36,1);
                line6Right.setFontSize(48,1);
            }

            if (page_title_text == "DEP/APP SPD" and current_page_prop == 3) {
                eachPageInit();
                pageTitle.setText("APPROACH SPEED");
                pageTitle.setColor(255,255,255,255);
                line1Left.setText("FLAPS 6");
                line1Left.setColor(255,255,255,255);
                line1Mid.setText("");
                line1Right.setText("RETURN ▶");
                line1Right.setColor(255,255,255,255);

                if (flp6_app_lmt <= 74) {
                    line2Left.setText(sprintf("%d", flp6_vref + flp6_app_lmt));
                } else {
                    line2Left.setText(sprintf("%d", flp6_app_lmt));
                }
                line2Left.setColor(0,174,255,255);
                line2Mid.setText("");
                line2Right.setText("");
                line3Left.setText("FLAPS 16");
                line3Left.setColor(255,255,255,255);
                line3Mid.setText("");
                line3Right.setText("");
                if (flp16_app_lmt <= 74) {
                    line4Left.setText(sprintf("%d", flp16_vref + flp16_app_lmt));
                } else {
                    line4Left.setText(sprintf("%d", flp16_app_lmt));
                }
                line4Left.setColor(0,174,255,255);
                line4Mid.setText("");
                line4Right.setText("");
                line5Left.setText("FLAPS 30");
                line5Left.setColor(255,255,255,255);
                line5Mid.setText("");
                line5Right.setText("");
                if (flp30_app_lmt <=74) {
                    line6Left.setText(sprintf("%d", flp30_vref + flp30_app_lmt));
                } else {
                    line6Left.setText(sprintf("%d", flp30_app_lmt));
                }
                line6Left.setColor(0,174,255,255);
                line6Mid.setText("");
                line6Right.setText("");

                var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
                if (R1_press_orig == 0 and press == 1) {
                    page_title_text = "PERFORMANCE INIT";
                    current_page_prop = 2;
                }
                R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");

                var flp6_app_use_vref = 0;
                var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                if (L1_press_orig == 0 and press == 1) {
                    var flp6_app_inp = num(scratch_pad_text) or -9999;
                    if ((flp6_app_inp >= 5 and flp6_app_inp <=74) or flp6_app_inp > flp6_vso) {
                        flp6_app_lmt = flp6_app_inp;
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1) {
                    var flp16_app_inp = num(scratch_pad_text) or -9999;
                    if ((flp16_app_inp >= 5 and flp16_app_inp <=74) or flp16_app_inp > flp16_vso) {
                        flp16_app_lmt = flp16_app_inp;
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                if (L3_press_orig == 0 and press == 1) {
                    var flp30_app_inp = num(scratch_pad_text) or -9999;
                    if ((flp30_app_inp >= 5 and flp30_app_inp <=74) or flp30_app_inp > flp30_vso) {
                        flp30_app_lmt = flp30_app_inp;
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");
            }
            if (page_title_text == "DEP/APP SPD" and current_page_prop == 4) {
                eachPageInit();
                pageTitle.setText("GO-AROUND SPEEDS");
                pageTitle.setColor(255,255,255,255);
                pageTitleRight.setText("");
                line1Left.setText("CLEAN");
                line1Left.setColor(255,255,255,255);
                line1Mid.setText("");
                line1Right.setText("RETURN ▶");
                line1Right.setColor(255,255,255,255);
                line2Left.setText(sprintf("%d", cln_ga_lmt));
                line2Left.setColor(0,174,255,255);
                line2Mid.setText("");
                line2Right.setText("");
                line3Left.setText("FLAPS 6");
                line3Left.setColor(255,255,255,255);
                line3Mid.setText("");
                line3Right.setText("FLAPS 30");
                line3Right.setColor(255,255,255,255);
                if (flp6_ga_lmt >= 5 and flp6_ga_lmt <= 74){
                    line4Left.setText(sprintf("%d", flp6_vref + flp6_ga_lmt));
                } else {
                    line4Left.setText(sprintf("%d", flp6_ga_lmt));
                }
                line4Left.setColor(0,174,255,255);
                line4Mid.setText("");
                if (flp30_ga_lmt >= 5 and flp30_ga_lmt <= 74) {
                    line4Right.setText(sprintf("%d", flp30_vref + flp30_ga_lmt));
                } else {
                    line4Right.setText(sprintf("%d", flp30_ga_lmt));
                }
                line4Right.setColor(0,174,255,255);
                line5Left.setText("FLAPS 16");
                line5Left.setColor(255,255,255,255);
                line5Mid.setText("");
                line5Right.setText("");
                if (flp16_ga_lmt >= 5 and flp16_ga_lmt <= 74) {
                    line6Left.setText(sprintf("%d", flp16_vref + flp16_ga_lmt));
                } else {
                    line6Left.setText(sprintf("%d", flp16_ga_lmt));
                }
                line6Left.setColor(0,174,255,255);
                line6Mid.setText("");
                line6Right.setText("");
                line7Left.setText("");
                line7Mid.setText("");
                line7Right.setText("");

                var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
                if (R1_press_orig == 0 and press == 1) {
                    page_title_text = "PERFORMANCE INIT";
                    current_page_prop = 2;
                }
                R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                if (L1_press_orig == 0 and press == 1) {
                    var ga_lmt_inp = num(scratch_pad_text) or -9999;
                    if (ga_lmt_inp > cln_vso) {
                        cln_ga_lmt = ga_lmt_inp;
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
                if (L2_press_orig == 0 and press == 1) {
                    var ga_lmt_inp = num(scratch_pad_text) or -9999;
                    if ((ga_lmt_inp >= 5 and ga_lmt_inp <= 74) or (ga_lmt_inp >  flp6_vso)) {
                        flp6_ga_lmt = ga_lmt_inp;
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

                var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
                if (L3_press_orig == 0 and press == 1) {
                    var ga_lmt_inp = num(scratch_pad_text) or -9999;
                    if ((ga_lmt_inp >= 5 and ga_lmt_inp <= 74) or (ga_lmt_inp >  flp16_vso)) {
                        flp16_ga_lmt = ga_lmt_inp;
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");

                var press = getprop("/instrumentation/fmz/fmz-key-R2-press");
                if (R2_press_orig == 0 and press == 1) {
                    var ga_lmt_inp = num(scratch_pad_text) or -9999;
                    if ((ga_lmt_inp >= 5 and ga_lmt_inp <= 74) or (ga_lmt_inp >  flp30_vso)) {
                        flp30_ga_lmt = ga_lmt_inp;
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                }
                R2_press_orig = getprop("/instrumentation/fmz/fmz-key-R2-press");
            }
        }
    }
    fmz_setlistener(pageDepAppSpd);
    var pageDepAppSpd_timer = maketimer(3, pageDepAppSpd);
    pageDepAppSpd_timer.start();

    var pageFuelReserve = func {
        if (page_title_text == "FUEL RESERVE") {
            eachPageInit();
            total_page_prop = 1;
            pageTitle.setText("FUEL RESERVE");
            pageTitle.setColor(255,255,255,255);
            pageTitleRight.setText("");
            currentPage.setText(sprintf("%d", current_page_prop));
            totalPage.setText(sprintf("%d", total_page_prop));
            line1Left.setText("");
            line1Left.setColor(255,255,255,255);
            line1Mid.setText("");
            line1Right.setText("RETURN ▶");
            line1Right.setColor(255,255,255,255);

            if (fuel_rsv_mode == "NBAA") {
                line2Left.setText("◀ NBAA [ACT]");
            } else {
                line2Left.setText("◀ NBAA");
            }
            line2Left.setColor(0,174,255,255);
            line2Mid.setText("");
            line2Right.setText("");
            line3Left.setText("");
            line3Mid.setText("");
            line3Right.setText("");
            
            if (fuel_rsv_lb == 0) {
                line4Left.setText("----- ʟʙ");
            } else if (fuel_rsv_lb > 0) {
                line4Left.setText(sprintf("%d", fuel_rsv_lb) ~ " ʟʙ");
            }
            line4Left.setColor(0,174,255,255);

            line4Mid.setText("");
            line4Right.setText("AT DEST");
            line4Right.setColor(255,255,255,255);
            line4Right.setFontSize(36);
            line5Left.setText("");
            line5Mid.setText("");
            line5Right.setText("OR ALTN IF");
            line5Right.setColor(255,255,255,255);

            if (fuel_rsv_min == 0) {
                line6Left.setText("--- ᴍɪɴ");
            } else if (fuel_rsv_min > 0) {
                line6Left.setText(sprintf("%d", fuel_rsv_min) ~ " ᴍɪɴ");
            }
            line6Left.setColor(0,174,255,255);
            
            line6Mid.setText("");
            line6Right.setText("ENTERED");
            line6Right.setColor(255,255,255,255);
            line6Right.setFontSize(36);

            var press = getprop("/instrumentation/fmz/fmz-key-R1-press");
            if (R1_press_orig == 0 and press == 1) {
                page_title_text = "PERFORMANCE INIT";
                current_page_prop = 3;
                line4Right.setFontSize(48);
                line6Right.setFontSize(48);
            }
            R1_press_orig = getprop("/instrumentation/fmz/fmz-key-R1-press");

            var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
            if (L1_press_orig == 0 and press == 1) {
                fuel_rsv_mode = "NBAA";
                fuel_rsv_lb = 0;
                fuel_rsv_min = 0;

                page_title_text = "PERFORMANCE INIT";
                current_page_prop = 3;
                line4Right.setFontSize(48);
                line6Right.setFontSize(48);
            }
            L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");

            var press = getprop("/instrumentation/fmz/fmz-key-L2-press");
            if (L2_press_orig == 0 and press == 1) {
                if (num(scratch_pad_text) != nil) {
                    if (altn_airport == "") {
                        fuel_rsv_mode = dest_airport;
                    } else {
                        fuel_rsv_mode = altn_airport;
                    }
                    fuel_rsv_lb = num(scratch_pad_text);
                    fuel_rsv_min = 0;

                    page_title_text = "PERFORMANCE INIT";
                    current_page_prop = 3;
                    line4Right.setFontSize(48);
                    line6Right.setFontSize(48);

                    scratch_pad_text = "";
                } else {
                    scratch_pad_message = "INVALID INPUT";
                }
            }
            L2_press_orig = getprop("/instrumentation/fmz/fmz-key-L2-press");

            var press = getprop("/instrumentation/fmz/fmz-key-L3-press");
            if (L3_press_orig == 0 and press == 1) {
                if (num(scratch_pad_text) != nil) {
                    if (altn_airport == "") {
                        fuel_rsv_mode = dest_airport;
                    } else {
                        fuel_rsv_mode = altn_airport;
                    }
                    fuel_rsv_lb = 0;
                    fuel_rsv_min = num(scratch_pad_text);

                    page_title_text = "PERFORMANCE INIT";
                    current_page_prop = 3;
                    line4Right.setFontSize(48);
                    line6Right.setFontSize(48);

                    scratch_pad_text = "";
                } else {
                    scratch_pad_message = "INVALID INPUT";
                }
            }
            L3_press_orig = getprop("/instrumentation/fmz/fmz-key-L3-press");
        }
    }
    fmz_setlistener(pageFuelReserve);

    var pagePerformanceData = func () {
        if (page_title_text == "PERF DATA") {
            eachPageInit();
            total_page_prop = 4;
            if (current_page_prop == 1){
                eachPageInit();
                pageTitle.setText(page_title_text);
                pageTitle.setColor(255,255,255,255);
                currentPage.setText(sprintf("%d", current_page_prop));
                totalPage.setText(sprintf("%d", total_page_prop));
                line1Left.setText("CRZ/CEIL ALT");

                var alt_preselector = getprop("autopilot/settings/target-altitude-ft");
                if (alt_preselector > crn_crz_alt) {
                    crn_crz_alt = alt_preselector;
                }
                var crn_crz_fl = "FL" ~ sprintf("%03d", crn_crz_alt / 100);
                var ceil_alt_fl = "FL" ~ sprintf("%03d", ceil_alt / 100);
                line2Left.setText(crn_crz_fl ~ "/" ~ ceil_alt_fl);
                line2Left.setColor(0,174,255,255);
                line1Right.setText("STEP INC");
                line2Right.setText(sprintf("%d", step_incm));
                line2Right.setColor(0,174,255,255);
                line3Left.setText("DEST");
                line3Right.setText("ALTN");
                line4Mid.setText("<-------- ETE ------->");
                var ete_dest_hrs = math.floor(ete_to_dest);
                var ete_dest_min = (ete_to_dest - ete_dest_hrs) * 60;
                line4Left.setText(sprintf("%02d", ete_dest_hrs) ~ "+" ~ sprintf("%02d", ete_dest_min));
                line4Left.setColor(0,174,255,255);
                line4Left.setFontSize(36,1);
                var ete_altn_hrs = math.floor(ete_to_altn);
                var ete_altn_min = (ete_to_altn - ete_altn_hrs) * 60;
                line4Right.setText(sprintf("%02d", ete_altn_hrs) ~ "+" ~ sprintf("%02d", ete_altn_min));
                line4Right.setColor(0,174,255,255);
                line4Right.setFontSize(36,1);
                line5Mid.setText("<----- FUEL REQ ----->");
                line5Left.setText(sprintf("%0.1f", (dest_fuel_prd / 1000)));
                line5Left.setColor(0,174,255,255);
                line5Left.setFontSize(36,1);
                line5Right.setText(sprintf("%0.1f", (altn_fuel_prd / 1000)));
                line5Right.setColor(0,174,255,255);
                line5Right.setFontSize(36,1);
                line6Mid.setText("<----- FUEL FOM ----->");
                line6Left.setText(sprintf("%0.1f", fuel_prd_fom_dest));
                line6Left.setColor(0,174,255,255);
                line6Left.setFontSize(36,1);
                line6Right.setText(sprintf("%0.1f", fuel_prd_fom_altn));
                line6Right.setColor(0,174,255,255);
                line6Right.setFontSize(36,1);
                line7Left.setText("◀ PERF INIT");
                line7Right.setText("DEPARTURE ▶");
                
                var press = getprop("/instrumentation/fmz/fmz-key-L1-press");
                if (L1_press_orig == 0 and press == 1) {
                    var inp_num = num(scratch_pad_text) or -65532;
                    if (inp_num > ceil_alt) {
                        inp_num = ceil_alt;
                    } else if (inp_num > 0) {
                        crn_crz_alt = inp_num;
                    } else {
                        var fl_pos = find("FL", scratch_pad_text);
                        var fl_size = size(scratch_pad_text);
                        if (fl_pos == 0 and fl_size == 5) {
                            var fl_num = substr(scratch_pad_text, 2);
                            fl_num = num(fl_num) or -65532;
                            if (fl_num > 0) {
                                crn_crz_alt = fl_num * 100;
                            } else {
                                scratch_pad_message = "INVALID INPUT";
                            }
                        }
                    }
                    scratch_pad_text = "";
                }
                L1_press_orig = getprop("/instrumentation/fmz/fmz-key-L1-press");
                
                key_prs("R1", func() {
                    var inp_num = num(scratch_pad_text) or -65532.12;
                    var int_part = int(inp_num);
                    
                    if ((int_part - inp_num) == 0) {
                        var check_int = 1;
                    } else {
                        var check_int = 0;
                    }
                    if (inp_num >= 0 and check_int == 1) {
                        step_incm = inp_num;
                    } else {
                        scratch_pad_message = "INVALID INPUT";
                    }
                    scratch_pad_text = "";
                });
            }

            if (current_page_prop == 2) {
                eachPageInit();
                pageTitle.setText(page_title_text);
                pageTitle.setColor(255,255,255,255);
                currentPage.setText(sprintf("%d", current_page_prop));
                totalPage.setText(sprintf("%d", total_page_prop));
                line1Left.setText("DEST");
                line1Left.setFontSize(36,1);
                line1Right.setText("ALTN");
                line1Right.setFontSize(36,1);
                var dist_dest = dist_to_dest();
                if (dist_dest >= 1000) {
                    line2Left.setText(sprintf("%4d", dist_dest));
                } else {
                    line2Left.setText(sprintf("%3.1f", dist_dest));
                }
                line2Left.setColor(0,255,0,255);
                line2Left.setFontSize(36,1);
                line2Mid.setText("<------- DIST ------->");
                var dist_altn = ttl_rte_dst();
                if (dist_altn >= 1000) {
                    line2Right.setText(sprintf("%4d", dist_altn));
                } else {
                    line2Right.setText(sprintf("%3.1f", dist_altn));
                }
                line2Right.setColor(0,255,0,255);
                line2Right.setFontSize(36,1);
                var on_ground = getprop("/fdm/jsbsim/gear/wow") or 1;
                var eta_dest_dst = 0;
                var wpt_num = rte_wpt_counter();
                var whl_cnt = to_waypoint + 1;
                while (whl_cnt < wpt_num) {
                    var leg_dst = grtCircDist(whl_cnt);
                    eta_dest_dst = eta_dest_dst + leg_dst;
                    whl_cnt = whl_cnt + 1;
                }

                eta_dest_dst = eta_dest_dst + dst_frm_acr(to_waypoint);
                var grd_spd = getprop("/velocities/groundspeed-kt") or 0;
                var ete_dest = eta_dest_dst / grd_spd;
                var now_hr = num(substr(time_text, 0, 2));
                var now_min = num(substr(time_text, 2, 2));
                now_min = now_min / 60;
                var now_dcm = now_hr + now_min;
                var eta_dest = now_dcm + ete_dest;
                var eta_dest_hr = math.fmod(math.floor(eta_dest), 24);
                var eta_dest_min = eta_dest - math.floor(eta_dest);
                eta_dest_min = eta_dest_min * 60;
                var eta_dest_text = sprintf("%02d", eta_dest_hr);
                eta_dest_text = eta_dest_text ~ sprintf("%02d", eta_dest_min);
                eta_dest_text = eta_dest_text ~ "Z";
                if (on_ground == 1) {
                    line3Left.setText("");
                } else {
                    line3Left.setText(eta_dest_text);                    
                }
                line3Left.setColor(0,174,255,255);
                line3Left.setFontSize(36,1);
                line3Mid.setText("<-------- ETA ------->");
                var eta_altn_dst = 0;
                var wpt_num = waypoints.size();
                var whl_cnt = to_waypoint + 1;
                while (whl_cnt < wpt_num) {
                    var leg_dst = grtCircDist(whl_cnt);
                    eta_altn_dst = eta_altn_dst + leg_dst;
                    whl_cnt = whl_cnt + 1;
                }

                eta_altn_dst = eta_altn_dst + dst_frm_acr(to_waypoint);
                var grd_spd = getprop("/velocities/groundspeed-kt") or 0;
                var ete_altn = eta_altn_dst / grd_spd;
                var now_hr = num(substr(time_text, 0, 2));
                var now_min = num(substr(time_text, 2, 2));
                now_min = now_min / 60;
                var now_dcm = now_hr + now_min;
                var eta_altn = now_dcm + ete_altn;
                var eta_altn_hr = math.fmod(math.floor(eta_altn), 24);
                var eta_altn_min = eta_altn - math.floor(eta_altn);
                eta_altn_min = eta_altn_min * 60;
                var eta_altn_text = sprintf("%02d", eta_altn_hr);
                eta_altn_text = eta_altn_text ~ sprintf("%02d", eta_altn_min);
                eta_altn_text = eta_altn_text ~ "Z";
                if (on_ground == 1) {
                    line3Right.setText("");
                } else {
                    line3Right.setText(eta_altn_text);
                }
                line3Right.setColor(0,174,255,255);
                line3Right.setFontSize(36,1);
                line4Mid.setText("<----- FUEL REM ----->");
                var engine0_flow_pph = getprop("/engines/engine[0]/fuel-flow_pph") or 0;
                var engine1_flow_pph = getprop("/engines/engine[1]/fuel-flow_pph") or 0;
                var total_flow_pph = engine0_flow_pph + engine1_flow_pph;
                var fuel_rmn_dtn = total_fuel_lbs_inp - (total_flow_pph * ete_dest);
                var fuel_rmn_dtn_grd = total_fuel_lbs_inp - dest_fuel_prd;
                if (on_ground == 1) {
                    var frdgk = fuel_rmn_dtn_grd / 1000;
                    line4Left.setText(sprintf("%0.1f", frdgk));
                } else {
                    var frdk = fuel_rmn_dtn / 1000;
                    line4Left.setText(sprintf("%0.1f", frdk));
                }
                line4Left.setColor(0,174,255,255);
                line4Left.setFontSize(36, 1);
                var fuel_rmn_atn = total_fuel_lbs_inp - (total_flow_pph * ete_altn);
                var fuel_rmn_atn_grd = total_fuel_lbs_inp - altn_fuel_prd;
                if (on_ground == 1) {
                    var fragk = fuel_rmn_atn_grd / 1000;
                    line4Right.setText(sprintf("%0.1f", fragk));
                } else {
                    var frak = fuel_rmn_atn / 1000;
                    line4Right.setText(sprintf("%0.1f", frak));
                }
                line4Right.setColor(0,174,255,255);
                line4Right.setFontSize(36, 1);
                line5Mid.setText("<----- GROSS WT ----->");
                var grs_wgh_dtn_grd = zfw + fuel_rmn_dtn_grd;
                var grs_wgh_dtn = zfw + fuel_rmn_dtn;
                var grs_wgh_atn_grd = zfw + fuel_rmn_atn_grd;
                var grs_wgh_atn = zfw + fuel_rmn_atn;
                if (on_ground == 1) {
                    var gwdgk = grs_wgh_dtn_grd / 1000;
                    line5Left.setText(sprintf("%0.1f", gwdgk));
                } else {
                    var gwdk = grs_wgh_dtn / 1000;
                    line5Left.setText(sprintf("%0.1f", gwdk));
                }
                line5Left.setColor(0,174,255,255);
                line5Left.setFontSize(36, 1);
                if (on_ground == 1) {
                    var gwagk = grs_wgh_atn_grd / 1000;
                    line5Right.setText(sprintf("%0.1f", gwagk));
                } else {
                    var gwak = grs_wgh_atn / 1000;
                    line5Right.setText(sprintf("%0.1f", gwak));
                }
                line5Right.setColor(0,174,255,255);
                line5Right.setFontSize(36, 1);
                line7Left.setText("◀ PERF INIT");
                line7Right.setText("DEPARTURE ▶");
            }
            
            if (current_page_prop == 3) {
                eachPageInit();
                pageTitle.setText(page_title_text);
                pageTitle.setColor(255,255,255,255);
                currentPage.setText(sprintf("%d", current_page_prop));
                totalPage.setText(sprintf("%d", total_page_prop));
                line1Left.setText("CRZ");
                line1Left.setFontSize(36, 1);
                line1Right.setText("TW");
                line1Right.setFontSize(36, 1);
                line1Mid.setText("<----- AVG WIND ----->");
                line3Left.setText("  DEST FUEL REMAINING");
                line3Left.setFontSize(36, 1);
                line4Left.setText("PREFLIGHT PLAN");
                line4Left.setFontSize(36, 1);
                var eta_dest_dst = 0;
                var wpt_num = rte_wpt_counter();
                var whl_cnt = to_waypoint + 1;
                while (whl_cnt < wpt_num) {
                    var leg_dst = grtCircDist(whl_cnt);
                    eta_dest_dst = eta_dest_dst + leg_dst;
                    whl_cnt = whl_cnt + 1;
                }
                eta_dest_dst = eta_dest_dst + dst_frm_acr(to_waypoint);
                var grd_spd = getprop("/velocities/groundspeed-kt") or 0;
                var ete_dest = eta_dest_dst / grd_spd;
                var engine0_flow_pph = getprop("/engines/engine[0]/fuel-flow_pph") or 0;
                var engine1_flow_pph = getprop("/engines/engine[1]/fuel-flow_pph") or 0;
                var total_flow_pph = engine0_flow_pph + engine1_flow_pph;
                var fuel_rmn_dtn = total_fuel_lbs_inp - (total_flow_pph * ete_dest);
                var fuel_rmn_dtn_grd = plan_fuel_ground - fuel_no_rsv;
                var on_ground = getprop("/fdm/jsbsim/gear/wow") or 1;
                var frz_fuel_rmn_k = fuel_rmn_dtn_grd / 1000;
                line4Right.setText(sprintf("%0.1f", frz_fuel_rmn_k));
                line4Right.setColor(0,174,255,255);
                line4Right.setFontSize(36, 1);
                line5Left.setText("UPDATED PLAN");
                line5Left.setFontSize(36, 1);
                if (on_ground == 1) {
                    line5Right.setText("");
                    line5Right.setFontSize(36, 1);
                } else {
                    fuel_rmn_dtn_k = fuel_rmn_dtn / 1000;
                    line5Right.setText(sprintf("%0.1f", fuel_rmn_dtn_k));
                    line5Right.setFontSize(36, 1);
                }
                line5Right.setColor(0,174,255,255);
                line6Left.setText("DIFFERENCE");
                line6Left.setFontSize(36, 1);
                if (on_ground == 1) {
                    line6Right.setText("");
                } else {
                    var pln_upd_dfr = frz_fuel_rmn_k - fuel_rmn_dtn_k;
                    line6Right.setText(sprintf("%0.1f", pln_upd_dfr));
                }
                line6Right.setColor(0,174,255,255);
                line7Left.setText("◀ PERF INIT");
                line7Right.setText("DEPARTURE ▶");
            }
            
            if (current_page_prop == 4) {
                eachPageInit();
                pageTitle.setText(page_title_text);
                pageTitle.setColor(255,255,255,255);
                currentPage.setText(sprintf("%d", current_page_prop));
                totalPage.setText(sprintf("%d", total_page_prop));
                if (fuel_rsv_mode == "NBAA") {
                    line1Mid.setText("NBAA FUEL RESERVE");
                    line2Left.setText("    AT DEST");
                    line2Left.setFontSize(36, 1);
                    line2Right.setText("200NM +    ");
                    line2Right.setFontSize(36, 1);
                    line3Mid.setFontSize(36, 1);
                    line3Mid.setText("30 MIN @ 5000 @ 200");
                } else if (fuel_rsv_lb != 0) {
                    line1Mid.setText("LBS FUEL RESERVE");
                    if (altn_airport == "")  {
                        line2Left.setText("    AT DEST");
                        line2Left.setFontSize(36, 1);
                        lb_str = sprintf("%d", fuel_rsv_lb) ~ " LBS";
                        line2Right.setText("lb_str    ");
                        line2Right.setFontSize(36, 1);
                        line3Mid.setText("");
                    } else {
                        line2Left.setText("    AT ALTN");
                        line2Left.setFontSize(36, 1);
                        lb_str = sprintf("%d", fuel_rsv_lb) ~ " LBS";
                        line2Right.setText("lb_str    ");
                        line2Right.setFontSize(36, 1);
                        line3Mid.setText("");
                    }
                } else if (fuel_rsv_min != 0) {
                    line1Mid.setText("MIN FUEL RESERVE");
                    if (altn_airport == "")  {
                        line2Left.setText("    AT DEST");
                        line2Left.setFontSize(36, 1);
                        min_str = sprintf("%d", fuel_rsv_min) ~ " MIN";
                        line2Right.setText("min_str    ");
                        line2Right.setFontSize(36, 1);
                        line3Mid.setText("");
                    } else {
                        line2Left.setText("    AT ALTN");
                        line2Left.setFontSize(36, 1);
                        min_str = sprintf("%d", fuel_rsv_min) ~ " MIN";
                        line2Right.setText("min_str    ");
                        line2Right.setFontSize(36, 1);
                        line3Mid.setText("");
                    }
                }

                var eta_dest_dst = 0;
                var wpt_num = rte_wpt_counter();
                var whl_cnt = to_waypoint + 1;
                while (whl_cnt < wpt_num) {
                    var leg_dst = grtCircDist(whl_cnt);
                    eta_dest_dst = eta_dest_dst + leg_dst;
                    whl_cnt = whl_cnt + 1;
                }
                eta_dest_dst = eta_dest_dst + dst_frm_acr(to_waypoint);
                var grd_spd = getprop("/velocities/groundspeed-kt") or 0;
                var ete_dest = eta_dest_dst / grd_spd;
                var engine0_flow_pph = getprop("/engines/engine[0]/fuel-flow_pph") or 0;
                var engine1_flow_pph = getprop("/engines/engine[1]/fuel-flow_pph") or 0;
                var total_flow_pph = engine0_flow_pph + engine1_flow_pph;
                var fuel_rmn_dtn = total_fuel_lbs_inp - (total_flow_pph * ete_dest);
                var fuel_rmn_dtn_grd = plan_fuel_ground - fuel_no_rsv;
                var on_ground = getprop("/fdm/jsbsim/gear/wow") or 1;
                var frz_fuel_rmn_k = fuel_rmn_dtn_grd / 1000;
                var req_rsv_k = req_rsv / 1000;
                line4Left.setText("REQ/PLAN");
                line4Left.setFontSize(36, 1);
                req_plan_txt = sprintf("%0.1f", req_rsv_k);
                req_plan_txt = req_plan_txt ~ "/";
                req_plan_txt = req_plan_txt ~ sprintf("%0.1f", frz_fuel_rmn_k);
                line4Right.setText();
                line4Right.setFontSize(36, 1);
                line4Right.setColor(0,174,255,255);
                
                line5Left.setText("UPDATED PLAN");
                line5Left.setFontSize(36, 1);
                if (on_ground == 1) {
                    line5Right.setText("");
                    line5Right.setFontSize(36, 1);
                } else {
                    fuel_rmn_dtn_k = fuel_rmn_dtn / 1000;
                    line5Right.setText(sprintf("%0.1f", fuel_rmn_dtn_k));
                    line5Right.setFontSize(36, 1);
                    line5Right.setColor(0,174,255,255);
                }

                line6Left.setText("DIFFERENCE");
                line6Left.setFontSize(36, 1);
                if (on_ground == 1) {
                    line6Right.setText("");
                } else {
                    var pln_upd_dfr = frz_fuel_rmn_k - fuel_rmn_dtn_k;
                    line6Right.setText(sprintf("%0.1f", pln_upd_dfr));
                    line6Right.setColor(0,174,255,255);
                }
                
                line7Left.setText("◀ PERF INIT");
                line7Right.setText("DEPARTURE ▶");
            }
            
            var press = getprop("/instrumentation/fmz/fmz-key-L4-press");
            if (L4_press_orig == 0 and press == 1) {
                page_title_text = "PERFORMANCE INIT";
            }
            L4_press_orig = getprop("/instrumentation/fmz/fmz-key-L4-press");

            var press = getprop("/instrumentation/fmz/fmz-key-R4-press");
            if (R4_press_orig == 0 and press == 1) {
                page_title_text = "DEPARTURE RUNWAYS";
            }
            R4_press_orig = getprop("/instrumentation/fmz/fmz-key-R4-press");
        }
    }
    fmz_setlistener(pagePerformanceData);
    var page_perf_data_timer = maketimer(1.0, pagePerformanceData);
    page_perf_data_timer.start();

    var saveLastPosition = maketimer(60, func() {
        var lat_deg = getprop("/position/latitude-deg");
        var lon_deg = getprop("/position/longitude-deg");
        var path = string.normpath(getprop("/sim/fg-home") ~ '/Export/bd700-last-pos.xml');
        var file = io.open(path, "w");
        var xml  = '<?xml version="1.0" encoding="UTF-8"?>
<LastPos>
  <lat>' ~ lat_deg ~ '</lat>
  <lon>' ~ lon_deg ~ '</lon>
</LastPos>';
        io.write(file, xml);
        io.close(file);

    });
    saveLastPosition.start();

    var waypointsCoodCalc = func {
        var R = 6371000;
        var wpt_num = waypoints.size();
        var mag_var_deg = getprop("/environment/magnetic-variation-deg") or 0;
        
        for (i=0; i<wpt_num; i=i+1) {
            var id = waypoints.vector[i].id;
            var type = waypoints.vector[i].type;
            if (type == "ConstHdgtoAlt") {
                var hdg = (waypoints.vector[i].hdg_crs_value + mag_var_deg) * D2R;
                var acft_lat = getprop("/position/latitude-deg") * D2R or 0;
                var acft_lon = getprop("/position/longitude-deg") * D2R or 0;
                var gs = getprop("/velocities/groundspeed-kt") or 0;
                var vs = getprop("/velocities/vertical-speed-fps") or 0;
                vs = vs * 60;
                var alt = getprop("/position/altitude-ft") or 0;
                var alt_req = waypoints.vector[i].altitude;
                var alt_delta = alt_req - alt;
                if (gs > 100 and vs > 1000) {
                    var ct_hr = (alt_delta / vs) / 60;
                    var dist = ct_hr * gs;
                } else {
                    var dist = (alt_delta / 2000 / 60) * 200;
                }
                dist = dist * 1852;
                var new_lat = math.asin(math.sin(acft_lat) * math.cos(dist/R) + math.cos(acft_lat) * math.sin(dist/R) * math.cos(hdg));
                var new_lon = acft_lon + math.atan2(math.sin(hdg) * math.sin(dist/R) * math.cos(acft_lat), math.cos(dist/R) - math.sin(acft_lat) * math.sin(new_lat));
                new_lat = new_lat * R2D;
                new_lon = new_lon * R2D;
                new_lon = math.fmod((new_lon + 540), 360) - 180;
                waypoints.vector[i].lat_nd = new_lat;
                waypoints.vector[i].lon_nd = new_lon;

            } else if (type == "VorRadialIntc") {
                var lat1 = waypoints.vector[i-1].lat_nd;
                var lon1 = waypoints.vector[i-1].lon_nd;
                var theta13 = waypoints.vector[i].hdg_crs_value + mag_var_deg;
                lat1 = lat1 * D2R;
                lon1 = lon1 * D2R;
                theta13 = theta13 * D2R;
                var lat2 = waypoints.vector[i].lat;
                var lon2 = waypoints.vector[i].lon;
                var theta23 = waypoints.vector[i].radialtoIntercept + mag_var_deg;
                lat2 = lat2 * D2R;
                lon2 = lon2 * D2R;
                theta23 = theta23 * D2R;
                var delta_lat = lat2 - lat1;
                var delta_lon = lon2 - lon1;
                var delta12 = 2 * math.asin(math.sqrt(math.pow(math.sin(delta_lat / 2), 2) + math.cos(lat1) * math.cos(lat2) * math.pow(math.sin(delta_lon / 2), 2)));
                var theta_a = math.acos((math.sin(lat2) - math.sin(lat1) * math.cos(delta12)) / (math.sin(delta12) * math.cos(lat1)));
                var theta_b = math.acos((math.sin(lat1) - math.sin(lat2) * math.cos(delta12)) / (math.sin(delta12) * math.cos(lat2)));
                if (math.sin(lon2 - lon1) > 0) {
                    var theta12 = theta_a;
                    var theta21 = 2 * math.pi - theta_b;
                } else {
                    var theta12 = 2 * math.pi - theta_a;
                    var theta21 = theta_b;
                }
                var alpha1 = math.fmod((theta13 - theta12 + math.pi), (2 * math.pi)) - math.pi;
                var alpha2 = math.fmod((theta21 - theta23 + math.pi), (2 * math.pi)) - math.pi;
                var alpha3 = math.acos(-math.cos(alpha1) * math.cos(alpha2) + math.sin(alpha1) * math.sin(alpha2) * math.cos(delta12));
                var delta13 = math.atan2(math.sin(delta12) * math.sin(alpha1) * math.sin(alpha2), math.cos(alpha2) + math.cos(alpha1) * math.cos(alpha3));
                var lat3 = math.asin(math.sin(lat1) * math.cos(delta13) + math.cos(lat1) * math.sin(delta13) * math.cos(theta13));
                var delta_lon13 = math.atan2(math.sin(theta13) * math.sin(delta13) * math.cos(lat1), math.cos(delta13) - math.sin(lat1) * math.sin(lat3));
                var lon3 = lon1 + delta_lon13;
                lat3 = lat3 * R2D;
                lon3 = lon3 * R2D;
                lon3 = math.fmod(lon3 + 540, 360) - 180;
                var old_name = waypoints.vector[i].orgn_id;
                var vor_radial = waypoints.vector[i].radialtoIntercept;
                new_name = old_name ~ "-R" ~ sprintf("%03d", vor_radial);
                waypoints.vector[i].id = new_name;
                waypoints.vector[i].lat_nd = lat3;
                waypoints.vector[i].lon_nd = lon3;              

            } else if (type == "DmeIntc") {
                var dst_fr_prv = 0;
                var delta_dst = 9999;
                while (dst_fr_prv < 600 and delta_dst >= 0.1) {
                    # Calculates a gussing point.
                    var phi1 = waypoints.vector[i-1].lat_nd * D2R;
                    var lambda1 = waypoints.vector[i-1].lon_nd * D2R;
                    var brng = (waypoints.vector[i].hdg_crs_value + mag_var_deg) * D2R;
                    var d_delta = dst_fr_prv * 1852;
                    var phi2 = math.asin(math.sin(phi1) * math.cos(d_delta / R) + math.cos(phi1) * math.sin(d_delta/R) * math.cos(brng));
                    var lambda2 = lambda1 + math.atan2(math.sin(brng) * math.sin(d_delta / R) * math.cos(phi1), math.cos(d_delta / R) - math.sin(phi1) * math.sin(phi2));
                    # Calculates the distance from the gussing point to the VOR/DME.
                    phi1 = phi2;
                    lambda1 = lambda2;
                    phi2 = waypoints.vector[i].lat * D2R;
                    lambda2 = waypoints.vector[i].lon * D2R;
                    var delta_phi = phi2 - phi1;
                    var delta_lambda = lambda2 - lambda1;
                    var a_alpha = math.pow(math.sin(delta_phi / 2), 2) + math.cos(phi1) * math.cos(phi2) * math.pow(math.sin(delta_lambda / 2), 2);
                    var c_charlie = 2 * math.atan2(math.sqrt(a_alpha), math.sqrt(1 - a_alpha));
                    var crt_dst_to_dme = R * c_charlie;
                    crt_dst_to_dme = crt_dst_to_dme / 1852;
                    var rqr_dst_to_dme = waypoints.vector[i].DMEtoIntercept;

                    delta_dst = math.abs(rqr_dst_to_dme - crt_dst_to_dme);
                    dst_fr_prv = dst_fr_prv + 0.1;
                }
                var new_lat = phi1 * R2D;
                var new_lon = lambda1 * R2D;
                new_lon = math.fmod(new_lon + 540, 360) - 180;
                waypoints.vector[i].lat_nd = new_lat;
                waypoints.vector[i].lon_nd = new_lon;
                var old_id = waypoints.vector[i].orgn_id;
                var vor_dme = waypoints.vector[i].DMEtoIntercept;
                vor_dme = sprintf("%d", vor_dme);
                var new_id = old_id ~ "-D" ~ vor_dme;
                waypoints.vector[i].id = new_id;
                

            } else if (type == "Intc") {
                var lat1 = waypoints.vector[i-1].lat_nd;
                var lon1 = waypoints.vector[i-1].lon_nd;
                var theta13 = waypoints.vector[i].hdg_crs_value + mag_var_deg;
                lat1 = lat1 * D2R;
                lon1 = lon1 * D2R;
                theta13 = theta13 * D2R;
                var lat2 = waypoints.vector[i].lat;
                var lon2 = waypoints.vector[i].lon;
                var theta23 = waypoints.vector[i].radialtoIntercept + mag_var_deg;
                lat2 = lat2 * D2R;
                lon2 = lon2 * D2R;
                theta23 = theta23 * D2R;
                var delta_lat = lat2 - lat1;
                var delta_lon = lon2 - lon1;
                var delta12 = 2 * math.asin(math.sqrt(math.pow(math.sin(delta_lat / 2), 2) + math.cos(lat1) * math.cos(lat2) * math.pow(math.sin(delta_lon / 2), 2)));
                var theta_a = math.acos((math.sin(lat2) - math.sin(lat1) * math.cos(delta12)) / (math.sin(delta12) * math.cos(lat1)));
                var theta_b = math.acos((math.sin(lat1) - math.sin(lat2) * math.cos(delta12)) / (math.sin(delta12) * math.cos(lat2)));
                if (math.sin(lon2 - lon1) > 0) {
                    var theta12 = theta_a;
                    var theta21 = 2 * math.pi - theta_b;
                } else {
                    var theta12 = 2 * math.pi - theta_a;
                    var theta21 = theta_b;
                }
                var alpha1 = math.fmod((theta13 - theta12 + math.pi), (2 * math.pi)) - math.pi;
                var alpha2 = math.fmod((theta21 - theta23 + math.pi), (2 * math.pi)) - math.pi;
                var alpha3 = math.acos(-math.cos(alpha1) * math.cos(alpha2) + math.sin(alpha1) * math.sin(alpha2) * math.cos(delta12));
                var delta13 = math.atan2(math.sin(delta12) * math.sin(alpha1) * math.sin(alpha2), math.cos(alpha2) + math.cos(alpha1) * math.cos(alpha3));
                var lat3 = math.asin(math.sin(lat1) * math.cos(delta13) + math.cos(lat1) * math.sin(delta13) * math.cos(theta13));
                var delta_lon13 = math.atan2(math.sin(theta13) * math.sin(delta13) * math.cos(lat1), math.cos(delta13) - math.sin(lat1) * math.sin(lat3));
                var lon3 = lon1 + delta_lon13;
                lat3 = lat3 * R2D;
                lon3 = lon3 * R2D;
                lon3 = math.fmod(lon3 + 540, 360) - 180;
                waypoints.vector[i].lat_nd = lat3;
                waypoints.vector[i].lon_nd = lon3;              
            } else if (type == "Vectors") {
                var hdg = (waypoints.vector[i].hdg_crs_value + mag_var_deg) * D2R;
                var acft_lat = getprop("/position/latitude-deg") * D2R or 0;
                var acft_lon = getprop("/position/longitude-deg") * D2R or 0;
                var dist = 9999 * 1852;
                var new_lat = math.asin(math.sin(acft_lat) * math.cos(dist/R) + math.cos(acft_lat) * math.sin(dist/R) * math.cos(hdg));
                var new_lon = acft_lon + math.atan2(math.sin(hdg) * math.sin(dist/R) * math.cos(acft_lat), math.cos(dist/R) - math.sin(acft_lat) * math.sin(new_lat));
                new_lat = new_lat * R2D;
                new_lon = new_lon * R2D;
                new_lon = math.fmod((new_lon + 540), 360) - 180;
                waypoints.vector[i].lat_nd = new_lat;
                waypoints.vector[i].lon_nd = new_lon;
            }
        }
    }
    fmz_setlistener(waypointsCoodCalc);
    var waypointsCoodCalc_timer = maketimer(0.382, waypointsCoodCalc);
    waypointsCoodCalc_timer.start();

    var prep_wp_num = waypoints.size();
    setprop("/instrumentation/fmz/waypoints-num", waypoints.size());
    var wpSizePropGen = func {
        var new_wp_num = waypoints.size();
        if (new_wp_num != prep_wp_num) {
            setprop("/instrumentation/fmz/waypoints-num", waypoints.size());
            prep_wp_num = new_wp_num;
        }
    }
    fmz_setlistener(wpSizePropGen);
    var wpSizePropGen_timer = maketimer(0.382, wpSizePropGen);
    wpSizePropGen_timer.start();

    my_canvas.addPlacement({"node": "fmz-screen"});

}

setlistener("sim/signals/fdm-initialized", fmzCanvas);
