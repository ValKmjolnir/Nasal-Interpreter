#// Wrappers for property I/O, for conveniently switching between two methods
var setProp = func(prop, value){
    #// setprop(prop, value);
    props.getNode("/").setValue(prop, value);
}
var getProp = func(prop){
    #// getprop(prop);
    props.getNode("/").getValue(prop);
}

var ChartsList =
{
    'KATL':
    {
        STAR: ["STAR_CANUK1_RNAV", "STAR_ERLIN9_RNAV", "STAR_FLCON7_RNAV", "STAR_HERKO6_RNAV", "STAR_HONIE8_RNAV", "STAR_LA_GRANGE2", "STAR_PECHY7_RNAV", "STAR_ROME4", "STAR_RPTOR1_RNAV", "STAR_SINCA5", "STAR_VIKNN_3RNAV", "STAR_WHINZ1"],
        STARs: 12,
        IAP: ["ILS_8L_Cat II - III", "ILS_PRM_8L", "ILS_PRM_8L_CAT II - III", "ILS_PRM_8R", "ILS_PRM_9L", "ILS-LOC_8L", "ILS-LOC_8R", "ILS-LOC_9L"],
        IAPs: 8,
        SID: ["SID_ATL5", "SID_BRAV6_RNAV", "SID_CADIT6_RNAV", "SID_COKEM5_RNAV", "SID_DAWGS5_RNAV", "SID_DOOLY5_RNAV", "SID_GEETK6_RNAV", "SID_JCKTS6_RNAV", "SID_JOGOR4_RNAV", "SID_MUNSN5_RNAV", "SID_NOVSS4_RNAV", "SID_NUGGT5_RNAV"],
        SIDs: 12,
        APT: "APT_0",
        APTs: 1,
        LOCATION: "ATLANTA USA",
        NAME: "Hartsfield-Jackson"
    },
    'KLAX':
    {
        STAR: "REDEYE2",
        STARs: 1,
        IAP: ["06L-ILS", "06R-ILS", "07L-ILS", "07R-ILS"],
        IAPs: 4,
        SID: "SNGO6",
        SIDs: 1,
        APT: "APT_0",
        APTs: 1,
        LOCATION: "LOS ANGELES USA",
        NAME: "Intntl"
    },
    'KIAH':
    {
        STAR: ["RIICE3_1", "RIICE_2"],
        STARs: 2,
        IAP: ["09-ILS", "15R-ILS"],
        IAPs: 2,
        SID: "",
        SIDs: 0,
        APT: "APT_0",
        APTs: 1,
        LOCATION: "HOUSTON USA",
        NAME: "G. Bush Intctl"
    },
    'VABB':
    {
        STAR: "",
        STARs: 0,
        IAP: ["09-ILS-DME", "14-ILS-DME"],
        IAPs: 2,
        SID: "EKPOSxA",
        SIDs: 1,
        APT: "APT_0",
        APTs: 1,
        LOCATION: "MUMBAI IN",
        NAME: "Chhatrapati Shivaji"
    },
    'KMEM':
    {
        STAR: "BEERT4",
        STARs: 1,
        IAP: "09-ILS_LOC",
        IAPs: 1,
        SID: "",
        SIDs: 0,
        APT: "APT_0",
        APTs: 1,
        LOCATION: "MEMPHIS USA",
        NAME: "International"
    },
    'LFRB':
    {
        STAR: "STAR",
        STARs: 1,
        IAP: ["IAP_25L", "IAP_25L_RNAV", "ILS_Y_25L", "ILS_YZ_07R", "ILS_Z_25L", "NDB_25L", "RADAR", "RNAV_07R", "VFR_APP01", "VFR_LNDG_01"],
        IAPs: 11,
        SID: ["SID_07R", "SID_25L"],
        SIDs: 2,
        APT: ["APT_0", "APT_1"],
        APTs: 2,
        LOCATION: "BREST FR",
        NAME: "Guipavas"
    },
    'LFRM':
    {
        STAR: ["VFR_APP01", "VFR_LND01"],
        STARs: 2,
        IAP: "",
        IAPs: 0,
        SID: "",
        SIDs: 0,
        APT: "APT_0",
        APTs: 1,
        LOCATION: "LE MANS FR",
        NAME: "Arnage"
    },
    'LIME':
    {
        STAR: ["STAR_ASTIG_ODINA_1S", "STAR_ASTIG_ODINA_1T", "STAR_DIXER_GEN_IDONA_1S", "STAR_DIXER_GEN_IDONA_1T", "STAR_DORIN_LUSIL_OSKOR_1S", "STAR_DORIN_LUSIL_OSKOR_1T", "STAR_East", "STAR_West", "STAR_VOR_DME", "VOR_Y28", "VOR_Y28_2", "VOR_Z28"],
        STARs: 12,
        IAP: ["ILS-PAPA-28", "ILS-PAPA-28-CAT II", "ILS-SIERRA-28", "ILS-SIERRA-28-CAT II", "ILS-TANGO-28", "ILS-TANGO-28-CAT II", "ILS-X28", "ILS-Y28", "ILS-Z28"],
        IAPs: 9,
        SID: ["SID_10_BEKAN_DORIN_5S", "SID_10-28_BEKAN5T_DORIN5T_ORI5QT", "SID_10-28_Alternate", "SID_28_SRN5R_TZO5R_TZO5S", "SID_InitialClimb", "SID10_InitialClimb", "SID28_InitialClimb", "SID28_InitialClimb2", "DEP_North_East", "DEP_South", "DEP_West"],
        SIDs: 12,
        APT: ["APT_0", "APT_0_Small", "GROUND_East_Apron", "GROUND_North_Apron", "GROUND_West_Apron"],
        APTs: 6,
        LOCATION: "BERGAMO IT",
        NAME: "Orio al Serio"
    },
    'LOWI':
    {
        STAR: ["26-RTT", "STAR", "VFR_East", "VFR_West_KTI", "West-NDB_KYI"],
        STARs: 5,
        IAP: ["LOC_26_RNAV", "LOC_DME_East", "LOC_DME_West", "NDB_East", "RNAV_08", "RNAV_26", "SPEC_LOC_DME_East"],
        IAPs: 8,
        SID: ["08-RTT", "General Departure", "SID_08", "SID0_RTT1W", "SID_08_SPECPERF_KPT1Z", "SID_08_SPECPERF_RTT2Z", "SID-26", "SID_26_MOGTI1H_RTT1Y", "SID_26_RNAV"],
        SIDs: 9,
        APT: ["APT_0", "APT_1"],
        APTs: 2,
        LOCATION: "INNSBRUCK AT",
        NAME: "Kranebitten"
    },
    'WSSS':
    {
        STAR: ["BOBAG1A", "BOBAG1B", "LAVAX1A", "LAVAX1B"],
        STARs: 4,
        IAP: ["02C-VOR-DME", "02L-ILS", "02L-ILS-CATII", "20C-VOR-DME", "20R-ILS"],
        IAPs: 5,
        SID: "BOG1",
        SIDs: 1,
        APT: ["APT_0", "APT_1"],
        APTs: 2,
        LOCATION: "SINGAPORE SG",
        NAME: "Changi"
    },
};

# used in page
var l=[];
var r=[];
for(var i=0;i<21;i+=1)
{
    append(l,"");
    append(r,"");
}
var KCl0_0 = "";
var KCl0_1 = "";
var KCl0   = "";
var KCl1   = "";
var KCl2   = "";
var KCl3   = "";
var KCl4   = "";
var KCr1   = "";
var KCr2   = "";
var KCr3   = "";
var KCr4   = "";
var KCr5   = "";
var KCr6   = "";
var ChartName = ["NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL", "NULL"];
var ChartDisp = ["NULL", "NULL", "NULL"];
var Keyboard_Helper = "";
var Keyboard_Message = "";
var Index_Max = 0;
var Index_Offset = 0;
var PageList = 0;

# used in efb
var ZoomFact = 0;
var PanHFact = 0;
var PanVFact = 0;
var OriginApt      = "NOT Set";
var DestinationApt = "NOT Set";
var ClosingFlightFlag = 0;
var lenght = 0;
var Temp_String = "";
var Cnv_Fact = 0;
var VK_Key = "";
var VK_Input_Mem  = 0;
var VK_Output_Mem = 0;
var TMZ_Input_Lenght     = 0;
var TMZ_Converted_ID     = "";
var TMZ_Converted_Name   = "";
var TMZ_Converted_OffSet = "";
var TMZ_zulu_HH = "";
var TMZ_Index = 0;
var TMZ_DB_Size = 0;
var Output_TMZ_Line = "";
var TMZ_DATE = "";
var TMZ_TIME = "";
var Output_HH = 0;
var Output_MM = 0;
var CUT = ["NULL", "NULL", "NULL"];
var Initial_FL = 0;
var Target_FL  = 0;
var Initial_GS = 0;
var Target_GS  = 0;
var DRC_String = "";
var DRC_Input_Lenght = 0;

# public vars
var helper = "";
var keypress = "";
var nochart = "";
var AptName = "Unknown";
var Chart_Pages = 1;
var DRC_Distance = 0;
var DRC_l3 = "";
var DRC_r3 = "";
var DRC_l5 = "";
var DRC_r5 = "";
var DRC_l7 = "";
var DRC_r8 = "";
var DRC_r9 = "";
var DRC_r10 ="";

#  Chart Properties 
ident = getProp("/sim/airport/closest-airport-id");
#AptName = getProp("/instrumentation/efb/chart/icao");

if (ident != nil)
{
    setProp("instrumentation/efb/chart/icao", ident);
    setProp("instrumentation/efb/chart/DEP_icao", getProp("/instrumentation/efb/chart/icao"));
} else setProp("/instrumentation/efb/chart/icao", "");
setProp("/instrumentation/efb/chart/type", "APT");
setProp("/instrumentation/efb/chart/newairport", 0);
setProp("/instrumentation/efb/FlightStatus", 0);        # INACTIVE
nochart = 0;

var page = {
    update : func {
        for(var i=0; i<21; i+=1)
            setProp("/instrumentation/efb/display/line"~i~"-l", l[i]);
        # r[0] haven't been used
        for(var i=1; i<21; i+=1)
            setProp("/instrumentation/efb/display/line"~i~"-r", r[i]);
        setProp("/instrumentation/efb/display/input-helper", helper);
        setProp("/instrumentation/efb/keypress", keypress);
        return;
    },
    clearpage : func {
        for(var i=0; i<21; i+=1){
            l[i] = "";
            r[i] = "";
        }
        helper = "";
        return;
    },
    index : func {
        for(var i=0; i<21; i+=1){
            l[i] = "";
            r[i] = "";
        }
        return;
    },
    index_TEST : func {
        for(var i=0; i<21; i+=1)
            l[i] = "< TEST_L"~i;
        # r[0] haven't been used
        for(var i=1; i<15; i+=1)
            r[i] = "TEST >";
        for(var i=15; i<21; i+=1)
            r[i] = "TEST_R"~i~" >";
        return;
    },
    KCupdate : func {
        setProp("/instrumentation/efb/display/lineAPT_NAME", KCl0);
        setProp("/instrumentation/efb/display/lineAPT_LOCATION", KCl0_0);
        setProp("/instrumentation/efb/display/lineAPT_PAGESHOW", KCl0_1);
        setProp("/instrumentation/efb/display/lineSTAR", KCl1);
        setProp("/instrumentation/efb/display/lineIAP", KCl2);
        setProp("/instrumentation/efb/display/lineSID", KCl3);
        setProp("/instrumentation/efb/display/lineAPT", KCl4);

        setProp("/instrumentation/efb/display/lineCHART1-r", KCr1);
        setProp("/instrumentation/efb/display/lineCHART2-r", KCr2);
        setProp("/instrumentation/efb/display/lineCHART3-r", KCr3);

        setProp("/instrumentation/efb/Keyboard/CHART_Input_Line", Keyboard_Helper);
        setProp("/instrumentation/efb/Keyboard/CHART_Output_Line", Keyboard_Message);
    },
    KCclearpage : func {
        KCl0 = "";
        KCl0_0 = "";
        KCl0_1 = "";
        KCl1 = "";
        KCl2 = "";
        KCl3 = "";
        KCl4 = "";

        KCr1 = "";
        KCr2 = "";
        KCr3 = "";

        Keyboard_Helper = "";
        Keyboard_Message = "";
        helper = "";
    },
    DRCupdate : func {
        setProp("/instrumentation/efb/display/DRC_l3", DRC_l3);
        setProp("/instrumentation/efb/display/DRC_l5", DRC_l5);
        setProp("/instrumentation/efb/display/DRC_r3", DRC_r3);
        setProp("/instrumentation/efb/display/DRC_r5", DRC_r5);
        setProp("/instrumentation/efb/display/DRC_l7", DRC_l7);
        setProp("/instrumentation/efb/display/DRC_r8", DRC_r8);
        setProp("/instrumentation/efb/display/DRC_r9", DRC_r9);
        setProp("/instrumentation/efb/display/DRC_r10", DRC_r10);
    },
    DRCclearpage : func {
        DRC_l3 = "";
        DRC_l5 = "";
        DRC_r3 = "";
        DRC_r5 = "";
        DRC_l7 = "";
        DRC_r8 = "";
        DRC_r9 = "";
        DRC_r10 = "";
    },
    airportinfo : func {
        var ClosestName = "";
        if (getProp("/instrumentation/gps/scratch/name") == nil)
            ClosestName = "";
        else
            ClosestName = getProp("/instrumentation/gps/scratch/name");
        l[2] = "Airport   : " ~ getProp("/instrumentation/gps/scratch/ident") ~ " (" ~ ClosestName ~ ")";
        l[3] = "Latitude  : " ~ sprintf("%3.4f", getProp("/instrumentation/gps/scratch/latitude-deg"));
        l[4] = "Longitude : " ~ sprintf("%3.4f", getProp("/instrumentation/gps/scratch/longitude-deg"));
        l[5] = "Elevation : " ~ sprintf("%3.4f", getProp("/instrumentation/gps/scratch/altitude-ft")) ~ " ft";
        setProp("/environment/metar[6]/station-id", getProp("/instrumentation/gps/scratch/ident"));
        l[7] = "Runways : ";
        l[8] = "         ";
        for (var n=0; n<12; n+=1) {
            if (getProp("/instrumentation/gps/scratch/runways[" ~ n ~ "]/id") != nil) {
                if (n <= 7) l[7] = l[7] ~ getProp("/instrumentation/gps/scratch/runways[" ~ n ~ "]/id") ~ " ";
                else l[8] = l[8] ~ getProp("/instrumentation/gps/scratch/runways[" ~ n ~ "]/id") ~ " ";
            }
            l[10] = "Weather Source: NOAA/METAR";
            r[10] = substr(getProp("/environment/metar[6]/data"),0,16) ~ "z";
            l[11] = substr(getProp("/environment/metar[6]/data"),17,41);
            if (size(getProp("/environment/metar[6]/data")) > 41)
                l[12] = substr(getProp("/environment/metar[6]/data"), 58, 41);
            l[13] = "           Current Weather Situation";
            l[14] = "Temperature : " ~ sprintf("%3.2f", getProp("/environment/metar[6]/temperature-degc")) ~ " �C";
            r[14] = "Dewpoint : " ~ sprintf("%3.2f", getProp("/environment/metar[6]/dewpoint-degc")) ~ " �C";
            l[15] = "Wind Direction : " ~ sprintf("%3.0f", getProp("/environment/metar[6]/base-wind-dir-deg")) ~ " degs";
            r[15] = "Wind Speed : " ~ sprintf("%3.0f", getProp("/environment/metar[6]/base-wind-speed-kt")) ~ " Kt";
            l[16] = "Max Visibility : " ~ sprintf("%3.0f", getProp("/environment/metar[6]/max-visibility-m")) ~ " ft";
            r[16] = "Min Visibility : " ~ sprintf("%3.0f", getProp("/environment/metar[6]/min-visibility-m")) ~ " ft";
            l[17] = "Relative Humidity : " ~ sprintf("%3.2f", getProp("/environment/metar[6]/rel-humidity-norm")) ~ " %";
            l[18] = "Pressure : " ~ sprintf("%3.2f", getProp("/environment/metar[6]/pressure-inhg")) ~ " inHg";
            l[20] = "Weather Report subject to Station availability";
        }
    },
    FlightMonitor : func {
        if (getProp("/instrumentation/efb/FlightStatus") == 1) {
            var FStatus = "ACTIVE";
        } else FStatus = "INACTIVE";
        if (getProp("/autopilot/route-manager/active") == 1) {
            if (getProp("/autopilot/route-manager/departure/takeoff-time") != nil) { 
                var TOT = getProp("/autopilot/route-manager/departure/takeoff-time") ~ "z";
            } else TOT = "Unknown";
            if (getProp("/autopilot/route-manager/destination/touchdown-time") != nil) { 
                var TDT = getProp("/autopilot/route-manager/destination/touchdown-time") ~ "z";
            } else TDT = "Unknown";
            var OAPT = getProp("/autopilot/route-manager/departure/airport") ~ " " ~ getProp("/autopilot/route-manager/departure/runway");
            var DAPT = getProp("/autopilot/route-manager/destination/airport") ~ " " ~ getProp("/autopilot/route-manager/destination/runway");
        } else { 
            OAPT = "NOT Set";
            DAPT = "NOT Set";
            TOT = "Unknown";
            TDT = "Unknown";
        }
        l[2]  = "Aircraft: " ~ getProp("/sim/description") ~ " Seattle";
        l[3]  = "Operator: " ~ substr(getProp("/sim/aircraft-operator"),0,3);
        r[3]  = "CallSign: " ~ substr(getProp("/sim/multiplay/callsign"), 0,6);
        l[4]  = "Flight No: " ~ "NOT Set"; # This will be set through CDU (To be done)
        r[4]  = "Flight Status: " ~ FStatus;
        l[5]  = "Origin APT: " ~ OAPT;
        r[5]  = "TO Time: " ~ TOT;
        l[6]  = "Destination APT: " ~ DAPT;
        r[6]  = "TD Time: " ~ TDT;
        l[7]  = "Date: " ~ substr(getProp("environment/metar/data"),0,10);
        r[7]  = "Time: " ~ getProp("/instrumentation/clock/indicated-short-string") ~ "z";
        l[9]  = "Ind. ALT: " ~ sprintf("%3.2f", getProp("/Instrumentation/altimeter/indicated-altitude-ft")) ~ " ft";
        r[9]  = "Press. ALT: " ~ sprintf("%3.2f", getProp("/Instrumentation/altimeter/pressure-alt-ft")) ~ " ft";
        l[10] = "Press. hPa: " ~ sprintf("%3.2f", getProp("/Instrumentation/altimeter/setting-hpa"));
        r[10] = "Press. inHg: " ~ sprintf("%3.2f", getProp("/Instrumentation/altimeter/setting-inhg"));
        l[11] = "Speed Knots: " ~ sprintf("%3.0f", getProp("/Instrumentation/airspeed/indicated-speed-kt"));
        r[11] = "Speed Mach: " ~ sprintf("%3.0f", getProp("/Instrumentation/airspeed/indicated-mach"));
        l[12] = "Temperature �C: " ~ sprintf("%3.2f", getProp("/environment/temperature-degc"));
        r[12] = "Temperature �F: " ~ sprintf("%3.2f", getProp("/environment/temperature-degf"));
        l[13] = "Wind Dir.: " ~ sprintf("%3.0f", getProp("/environment/metar/base-wind-dir-deg")) ~ " degs";
        r[13] = "Wind Speed: " ~ sprintf("%3.0f", getProp("/environment/base-wind-speed-kt")) ~ " kts";
        l[14] = "Total Fuel: " ~ sprintf("%3.2f", getProp("/consumables/fuel/total-fuel-gals")) ~ " gals";
        l[15] = "Gross Weight: " ~ sprintf("%3.2f", getProp("/yasim/gross-weight-lbs")) ~ " lbs";
        # warning: this line l[14] has been changed to l[15],be careful of bugs.But it must be at the right side
    },
    gps : func {
        l[1]  = "         GPS POSITION / SETTINGS";
        l[2]  = "Latitude  : " ~ sprintf("%3.2f", getProp("/instrumentation/gps/indicated-latitude-deg")) ~ " degs";
        r[2]  = "Longitude : " ~ sprintf("%3.2f", getProp("/instrumentation/gps/indicated-longitude-deg")) ~ " degs";
        l[13] = "GPS Mode: " ~ getProp("/instrumentation/gps/mode");
        r[13] = "WayPoint ID: " ~ getProp("/instrumentation/gps/wp/wp[1]/ID");
        l[14] = "Leg Distance: " ~ sprintf("%3.2f", getProp("/instrumentation/gps/wp/leg-distance-nm")) ~ " Nm";
        l[15] = "Leg Magnetic Course : " ~ sprintf("%3.2f", getProp("/instrumentation/gps/wp/leg-mag-course-deg")) ~ " degs";
        l[16] = "Leg True Heading: " ~ sprintf("%3.2f", getProp("/instrumentation/gps/wp/leg-true-course-deg")) ~ " degs";
    },
    charts_keyboard : func {
        setProp("/instrumentation/efb/chart/Status", "ON");
        setProp("/instrumentation/efb/chart/type", "APT");
        PageList = getProp("/instrumentation/efb/chart/PageList");

        # Checks existence of Charts in the DB by chart's type ~ "-0"; example: "KSFO/type-0"
        var Chart_Search = "";
        Chart_Search = sprintf("%s", getProp("/instrumentation/efb/chart/icao"));

        # HERE we need to launch the SEARCH into the ChartsList hash
        #if (getProp("/instrumentation/efb/chartsDB/" ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ "/" ~ getProp("/instrumentation/efb/chart/type") ~ "-0") == nil ) { 
        #   setProp("/instrumentation/efb/chart_Found", "NOT_FOUND");
        #   nochart = 1;
        #} else { 
        #    setProp("/instrumentation/efb/chart_Found", "FOUND");
        #   nochart = 0;
        #}
        
        setProp("/instrumentation/efb/Keyboard/CHART_Input_Line", "Charts for " ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ ": " ~ getProp("/instrumentation/efb/chart_Found"));
        if (nochart == 0) {
            # gets the APT's Name & Location
            setProp("/instrumentation/efb/chart/type", "NAME");
            KCl0 = getProp("/instrumentation/efb/chartsDB/" ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ "/" ~ getProp("/instrumentation/efb/chart/type"));
            setProp("/instrumentation/efb/chart/type", "LOCATION");
            KCl0_0 = getProp("/instrumentation/efb/chartsDB/" ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ "/" ~ getProp("/instrumentation/efb/chart/type"));

            setProp("/instrumentation/efb/chart/type", "APT");
            # Gets the number of charts (by Type) in the Charts DB
            STAR_Status = setProp("/instrumentation/efb/chart/NumSTARs", (getProp("/instrumentation/efb/chartsDB/" ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ "/" ~ "STARs")));
            IAP_Status = setProp("/instrumentation/efb/chart/NumIAPs", (getProp("/instrumentation/efb/chartsDB/" ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ "/" ~ "IAPs")));
            SID_Status = setProp("/instrumentation/efb/chart/NumSIDs", (getProp("/instrumentation/efb/chartsDB/" ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ "/" ~ "SIDs")));
            APT_Status = setProp("/instrumentation/efb/chart/NumAPTs", (getProp("/instrumentation/efb/chartsDB/" ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ "/" ~ "APTs")));
    
            # Gets the Type of chart from Pilot's Selection

            if (getProp("/instrumentation/efb/VK_Chart_Type") != "") {
                if (getProp("/instrumentation/efb/VK_Chart_Type") == "STAR") {
                    setProp("/instrumentation/efb/chart/type", "STAR");
                    Keyboard_Message = getProp("/instrumentation/efb/chart/NumSTARs") ~ " " ~ getProp("/instrumentation/efb/chart/type") ~ " Charts for " ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ " Apt - Select a Chart";
                    STAR_Status = 1;
                } else STAR_STATUS = 0;
                if (getProp("/instrumentation/efb/VK_Chart_Type") == "IAP") {
                    setProp("/instrumentation/efb/chart/type", "IAP");
                    Keyboard_Message = getProp("/instrumentation/efb/chart/NumIAPs") ~ " " ~ getProp("/instrumentation/efb/chart/type") ~ " Charts for " ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ " Apt - Select a Chart";
                    IAP_Status = 1;
                } else IAP_Status = 0;
                if (getProp("/instrumentation/efb/VK_Chart_Type") == "SID") {
                    setProp("/instrumentation/efb/chart/type", "SID");
                    Keyboard_Message = getProp("/instrumentation/efb/chart/NumSIDs") ~ " " ~ getProp("/instrumentation/efb/chart/type") ~ " Charts for " ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ " Apt - Select a Chart";
                    SID_Status = 1;
                } else SID_Status = 0;
                if (getProp("/instrumentation/efb/VK_Chart_Type") == "APT") {
                    setProp("/instrumentation/efb/chart/type", "APT");
                    Keyboard_Message = getProp("/instrumentation/efb/chart/NumAPTs") ~ " " ~ getProp("/instrumentation/efb/chart/type") ~ " Charts for " ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ " Apt - Select a Chart";
                    APT_Status = 1;
                } else APT_Status = 0;
        
                setProp("/instrumentation/efb/VK_Chart_Type", getProp("/instrumentation/efb/chart/type"));
                setProp("/instrumentation/efb/chartsDB", "ChartsDB/");
        
                #setProp("/instrumentation/efb/chart/selected", getProp("/instrumentation/efb/chartsDB/" ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ "/" ~ getProp("/instrumentation/efb/chart/type") ~ "-1"));

                var lastn = getProp("/instrumentation/efb/chartsDB/" ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ "/" ~ getProp("/instrumentation/efb/chart/type") ~ "s") - 1;
                if (lastn <= 0) lastn = 0;
        
                if ((STAR_Status == 0) and (IAP_Status == 0) and (SID_Status == 0) and (APT_Status == 0)) { 
                    setProp("/instrumentation/efb/chart/Status", 0);
                    Keyboard_Message = "No Charts available for: " ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ " Apt";
                    setProp("/instrumentation/efb/chart/Status", "OFF");
                }

                # Gets the available ICAO/Type charts in the Charts DB
                for (var index = 0; index <= 29; index += 1)
                    ChartName[index] = "";
                
                if (lastn > 0) {
                    for (var index = 0; index <= lastn; index += 1) {
                        setProp("/instrumentation/efb/chart/IDX", index);
                        setProp("/instrumentation/efb/chart/selected", (getProp("/instrumentation/efb/chartsDB/" ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ "/" ~ getProp("/instrumentation/efb/chart/type") ~ "-" ~ getProp("/instrumentation/efb/chart/IDX"))));
                        ChartName[index] = getProp("/instrumentation/efb/chart/selected");
                    }
                } else { 
                    setProp("/instrumentation/efb/chart/selected", (getProp("/instrumentation/efb/chartsDB/" ~ sprintf("%s", getProp("/instrumentation/efb/chart/icao")) ~ "/" ~ getProp("/instrumentation/efb/chart/type") ~ "-0")));
                    ChartName[0] = getProp("/instrumentation/efb/chart/selected");
                }
                Keyboard_Helper = "Enter Airport ICAO";
                setProp("/instrumentation/efb/Keyboard/CHART_Input_Line", Keyboard_Helper);
                setProp("/instrumentation/efb/Keyboard/CHART_Output_Line", Keyboard_Message);

                # Prints Charts values to Upper Input Display
                if (getProp("/instrumentation/efb/chart/Status") == "ON") {
                    KCl1 = sprintf(getProp("/instrumentation/efb/chart/NumSTARs"));
                    KCl2 = sprintf(getProp ("/instrumentation/efb/chart/NumIAPs"));
                    KCl3 = sprintf(getProp ("/instrumentation/efb/chart/NumSIDs"));
                    KCl4 = sprintf(getProp ("/instrumentation/efb/chart/NumAPTs"));
            
                    # Set Index Offset for proper Page Display (by 3 rows)
                    if (lastn <=2)                        Index_Max = 0;
                    elsif ((lastn >  2) and (lastn <= 5)) Index_Max = 3;
                    elsif ((lastn >  5) and (lastn <= 8)) Index_Max = 6;
                    elsif ((lastn >  8) and (lastn <=11)) Index_Max = 9;
                    elsif ((lastn > 11) and (lastn <=14)) Index_Max = 12;
                    elsif ((lastn > 14) and (lastn <=17)) Index_Max = 15;
                    elsif ((lastn > 17) and (lastn <=20)) Index_Max = 18;
                    elsif ((lastn > 20) and (lastn <=23)) Index_Max = 21;
                    elsif ((lastn > 23) and (lastn <=26)) Index_Max = 24;
                    elsif ((lastn > 26) and (lastn <=29)) Index_Max = 27;
        
                    Chart_Pages = (Index_Max/3) + 1;

                    # Displays Actual Chart Page/Total Pages
                    var Page_Show = "Page " ~ getProp("/instrumentation/efb/chart/PageList") ~ "/" ~ Chart_Pages;
                    setProp("instrumentation/efb/chart/PageShow", Page_Show);
                    KCl0_1 = getProp("/instrumentation/efb/chart/PageShow");

                    # Build 3 rows of Chart Names
                    for (var index = 0; index <= 2; index += 1) {
                        Index_Offset = ((getProp("/instrumentation/efb/chart/PageList") - 1) * 3);
                        Index_Offset = Index_Offset + index;
                        ChartDisp[index] = ChartName[Index_Offset];
                    }
                    # Displays 3 Chart Names [MAX Lenght = 27 chars !], ready for Selection by the Pilot
                    setProp("instrumentation/efb/chart/Selection_0", ChartDisp[0]);
                    setProp("instrumentation/efb/chart/Selection_1", ChartDisp[1]);
                    setProp("instrumentation/efb/chart/Selection_2", ChartDisp[2]);
                    KCr1 = substr(getProp("instrumentation/efb/chart/Selection_0"), 0, 26);
                    KCr2 = substr(getProp("instrumentation/efb/chart/Selection_1"), 0, 26);
                    KCr3 = substr(getProp("instrumentation/efb/chart/Selection_2"), 0, 26);
                } else {
                    KCl0 = "";
                    KCl0_0 = "";
                    KCl0_1 = "";
                    KCl1 = "";
                    KCl2 = "";
                    KCl3 = "";
                    KCl4 = "";

                    KCr1 = "";
                    KCr2 = "";
                    KCr3 = "";
                    setProp("instrumentation/efb/chart/Selection_0", "");
                    setProp("instrumentation/efb/chart/Selection_1", "");
                    setProp("instrumentation/efb/chart/Selection_2", "");
                }
                page.KCupdate();
            }
        } # END of 'nochart == 0' Brace
    }
};

var efb = {
    init : func {
        me.UPDATE_INTERVAL = 0.05;
        me.loopid = 0;
        # INITIALIZE

        setProp("/instrumentation/efb/page", "MENU");
        setProp("/sim/model/efb/page", "Displays/Main_Menu.jpg");

        # Chart Section Stuff

        setProp("/instrumentation/efb/chart/type", "APT");
        setProp("/instrumentation/efb/chart/selected", "APT_0");
        setProp("/instrumentation/efb/VK_keypress", "");
        setProp("/instrumentation/efb/Keyboard/CHART_Input_Line", "");
        setProp("/instrumentation/efb/chart/rotation", 0);
        setProp("/instrumentation/efb/diagram/rotation", 0);

        AptName = getProp("/instrumentation/efb/chart/icao");

        # Various Stuff

        setProp("/instrumentation/efb/manual-page", 0);

        setProp("/instrumentation/efb/vnav_autogen/first", 0);
        setProp("/instrumentation/efb/vnav_autogen/gen", 0);
        TMZ_DB_Size = getProp("/instrumentation/efb/TimeZonesDB/TMZ_DB_Size") - 1;
        setProp("/instrumentation/efb/chart/PageList", 1);
        me.reset();
    },
    searchairport : func(query) {
        setProp("/instrumentation/gps/scratch/query", query);
        setProp("/instrumentation/gps/scratch/type", "airport");
        setProp("/instrumentation/gps/command", "search");
        setProp("/instrumentation/efb/selected-rwy/id", "");
    },
    searchcomms : func(query) {
        setProp("/sim/gui/dialogs/scratch/airports/selected-airport/id", query);
        #setProp("/sim/gui/dialogs/airports/scratch/type", "airport");
        setProp("/sim/gui/dialogs/scratch/airports/mode", "search");
        #setProp("/instrumentation/efb/selected-rwy/id", "");
    },
    searchcharts : func(chart) {setProp("/sim/model/efb/chart", "Charts/" ~ chart ~ ".jpg");},
    efbTimer : nil,
    update : func {
        var keypress = getProp("/instrumentation/efb/keypress");
        var currentPage = getProp("/instrumentation/efb/page");
        if (currentPage == "MENU") {
            # Make sure we know the APT's ICAO
            setProp("/instrumentation/gps/scratch/ident", getProp("/sim/airport/closest-airport-id"));

            if (getProp("/instrumentation/gps/scratch/ident") != nil) setProp("/instrumentation/efb/chart/icao", sprintf("%s", getProp("/instrumentation/gps/scratch/ident")));
            else setProp("/instrumentation/efb/chart/icao", "");
            
            AptName = sprintf("%s", getProp("/instrumentation/efb/chart/icao"));
            page.clearpage();
            page.index();
            
            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/Main_Menu.jpg");
                keypress = "";
            }
            elsif (keypress == "l1") { 
                setProp("/instrumentation/efb/page", "CHARTS");
                setProp("/instrumentation/efb/chart/chartmenu", 0);
                setProp("/instrumentation/efb/chart/zoom-in", 0);
                ZoomFact = 0;
                PanHFact = 0;
                PanVFact = 0;
                keypress = "";
            }
            elsif (keypress == "r1") { 
                setProp("/sim/model/efb/page", "Displays/Video_1.jpg");
                setProp("/instrumentation/efb/page", "VIDEO");
                keypress = "";
            }
            elsif (keypress == "l2") { 
                setProp("/sim/model/efb/page", "Displays/Performance_1.jpg");
                setProp("/instrumentation/efb/page", "PERFORMANCE");
                keypress = "";
            }
            elsif (keypress == "r2") { 
                setProp("/sim/model/efb/page", "Displays/Documents_1.jpg");
                setProp("/instrumentation/efb/page", "DOCUMENTS");
                keypress = "";
            }
            elsif (keypress == "l4") { 
                setProp("/sim/model/efb/page", "Displays/Airport_Info.jpg");
                setProp("/instrumentation/efb/page", "APT INFO");
                keypress = "";
            }
            elsif (keypress == "l5") { 
                setProp("/instrumentation/efb/page", "Airport Charts");
                keypress = "";
            }
            elsif (keypress == "l7") { 
                setProp("/sim/model/efb/page", "Displays/IDENT_1.jpg");
                setProp("/instrumentation/efb/page", "IDENT");
                keypress = "";
            }
            elsif (keypress == "l8") { 
                setProp("/sim/model/efb/page", "Displays/System_1.jpg");
                setProp("/instrumentation/efb/page", "SYSTEMS");
                keypress = "";
            }
            elsif (keypress == "r4") { 
                #//setProp("/instrumentation/efb/page", "Airport Diagram");
                keypress = "";
            }
            elsif (keypress == "r5") { 
                setProp("/sim/model/efb/page", "Displays/PU_1.jpg");
                setProp("/instrumentation/efb/page", "UTILITIES");
                keypress = "";
            }
            elsif (keypress == "r6") { 
                #//setProp("/instrumentation/efb/page", "Flight Fuel Planner");
                keypress = "";
            }
            elsif (keypress == "r7") { 
                setProp("/sim/model/efb/page", "Displays/Monitor.jpg");
                setProp("/instrumentation/efb/page", "MONITOR");
                keypress = "";
            }
            elsif (keypress == "r8") { 
                setProp("/instrumentation/efb/page", "INITIALIZE");
                keypress = "";
            }
        #__________________________________________________________________________________________
        # CHART SECTION Parser --------------------------------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "CHARTS") {
            page.clearpage();
            #setProp("/sim/model/efb/chart", "Displays/Help_1.jpg");
            setProp("/instrumentation/efb/diagram/rotation", 0);
            setProp("/instrumentation/efb/diagram/chartmenu", 0);
            setProp("/instrumentation/efb/chartsDB", "Charts/");
            #helper = "";
            # NOTE: this gives the Airport's name to be used on the bottom line of the Chart Display) has still to be checked: if RouteManager is not selected from the upper MENU, the property is not active...
            AptName = getProp("/instrumentation/efb/chart/icao");

            #_____________________________
            # Chart Selection at Airport  |
            #_____________________________|
            
            # Checks existence of Charts in the DB

            if(ChartsList[AptName] == nil){
                setProp("/instrumentation/efb/chart_Found", "NOT_FOUND");
                nochart = 1;
            }else{
                setProp("/instrumentation/efb/chart_Found", "FOUND");
                nochart = 0;
            }
            if (nochart == 0){
                page.update();
                if (getProp("/instrumentation/efb/chart/type") == "APT"){
                    l[0] = "AIRPORT MAP";
                }
                else l[0] = "";
            
                l[20] = AptName;
                efb.searchcharts(AptName ~ "/" ~ getProp("/instrumentation/efb/chart/type") ~ "/" ~ getProp("/instrumentation/efb/chart/selected"));
                #efb.searchcharts(AptName ~ "/" ~ getProp("/instrumentation/efb/chart/type") ~ "/" ~ getProp("instrumentation/efb/chart/selection2");
                setProp("/sim/model/efb/chart", getProp("/sim/model/efb/chart"));
                setProp("/sim/model/efb/chart_BKUP", getProp("/sim/model/efb/chart"));
                setProp("/sim/model/efb/Chart_2", getProp("/sim/model/efb/chart_BKUP"));
                setProp("/sim/model/efb/Chart_4", getProp("/sim/model/efb/chart_BKUP"));
                setProp("/sim/model/efb/Ovlay_1", "Displays/drawing.png");
            
                if (getProp("/instrumentation/efb/chart/chartmenu") == 0) setProp("/sim/model/efb/Ovlay_1", "Displays/drawing.png");
                if (getProp("/instrumentation/efb/chart/chartmenu") == 1) setProp("/sim/model/efb/Ovlay_1", "Displays/drawing2.png");
            
            } else { 
                setProp("/sim/model/efb/chart", "Displays/NoCharts.jpg");
                setProp("/sim/model/efb/Ovlay_1", "Displays/drawingNULL.png");
                helper = "Airport:    " ~ AptName;
            }
            # Handles the Zoom-In and Zoom-Out EFB buttons on Charts Display; available ZoomFact is 2x and 4x only.
            if ((ZoomFact >= 0) and (ZoomFact <= 4)) {
                if ((keypress == "Zin") and (nochart == 0)) { 
                    ZoomFact = ZoomFact + 2;
                    setProp("/instrumentation/efb/chart/zoom-in", ZoomFact);
                    keypress = "";
                }
                elsif ((keypress == "Zout") and (nochart == 0) and (ZoomFact > 0)) { 
                    ZoomFact = ZoomFact - 2;
                    setProp("/instrumentation/efb/chart/zoom-in", ZoomFact);
                    keypress = "";
                }
            }
            if (ZoomFact == 0) setProp("/instrumentation/efb/chart/zoom-in", 0);
            # Handles the Pan-Right, Pan_Left, ScrollUP and ScrollDN EFB buttons on Charts Display; Pan Horizontal Factor is 100; Pan Vertical Factor is 100.
            if ((ZoomFact == 2) or (ZoomFact == 4)) {
                if ((keypress == "Move_R") and (nochart == 0)) { 
                    PanHFact = PanHFact + 100;
                    setProp("/instrumentation/efb/chart/panH", PanHFact);
                    keypress = "";
                }
                elsif ((keypress == "Move_L") and (nochart == 0)) { 
                    PanHFact = PanHFact - 100;
                    setProp("/instrumentation/efb/chart/panH", PanHFact);
                    keypress = "";
                }
                elsif ((keypress == "ScrollUP") and (nochart == 0)) { 
                    PanVFact = PanVFact + 100;
                    setProp("/instrumentation/efb/chart/panV", PanVFact);
                    keypress = "";
                }
                elsif ((keypress == "ScrollDN") and (nochart == 0)) { 
                    PanVFact = PanVFact - 100;
                    setProp("/instrumentation/efb/chart/panV", PanVFact);
                    keypress = "";
                }
            } elsif (ZoomFact == 0) { 
                setProp("/instrumentation/efb/chart/panH", -PanHFact);
                setProp("/instrumentation/efb/chart/panV", -PanVFact);
            }
            # Handles Chart Rotation counter-clockwise
            if ((keypress == "l2") and (nochart == 0) and (getProp("/instrumentation/efb/chart/chartmenu")) == 0) toggle("/instrumentation/efb/chart/rotation");
            # Toggles Overlay Menu On/OFF
            if ((keypress == "r8")  and (nochart == 0)) toggle("/instrumentation/efb/chart/chartmenu");
            # Calls Virtual Keyboard for Chart DB Selection by the Pilot
            if (keypress == "l5") { 
                setProp("/instrumentation/efb/page", "CHARTS_KEYBOARD");
                setProp("/instrumentation/efb/chart/zoom-in", 0);
                setProp("/instrumentation/efb/VKDRC_keypress", "");
                setProp("/instrumentation/efb/VK_Keyboard/Input_String", "");
                setProp("/instrumentation/efb/chart/Searchable", 0);
            }
            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/Main_Menu.jpg");
                setProp("/instrumentation/efb/chart/zoom-in", 0);
                ZoomFact = 0;
                PanHFact = -PanHFact;
                PanVFact = -PanVFact;
                setProp("/instrumentation/efb/page", "MENU");
                keypress = "";
            }
        #__________________________________________________________________________________________
        # AIRPORT INFO SECTION Parser -------------------------------------------------------------|
        #__________________________________________________________________________________________|   
        } elsif (currentPage == "APT INFO") {
            
            page.clearpage();
            efb.searchairport(getProp("/sim/airport/closest-airport-id"));
            efb.searchcomms(getProp("/sim/airport/closest-airport-id"));
            page.airportinfo();
            page.update();

            # if (keypress == "r6") { 
            #   setProp("/instrumentation/efb/page", "Runway Information");
            #   keypress = "";
            # }
            if (keypress == "r8") { 
                setProp("/instrumentation/efb/page", "CHARTS");
                keypress = "";
            }
            elsif (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/Main_Menu.jpg");
                setProp("/instrumentation/efb/page", "MENU");
                keypress = "";
            }
        #__________________________________________________________________________________________
        # MONITOR SECTION Parser ------------------------------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "MONITOR") {
            page.clearpage();
            page.FlightMonitor();
            page.update();

            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/Main_Menu.jpg");
                setProp("/instrumentation/efb/page", "MENU");
                keypress = "";
            }
        #__________________________________________________________________________________________
        # VIDEO SECTION Parser --------------------------------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "VIDEO") {
            page.clearpage();
            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/Main_Menu.jpg");
                setProp("/instrumentation/efb/page", "MENU");
                keypress = "";
            }
        #__________________________________________________________________________________________
        # DOCUMENTS SECTION Parser ----------------------------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "DOCUMENTS") {
                page.clearpage();

                if (keypress == "l3") { 
                    setProp("/instrumentation/efb/page", "NORM PROC MANUAL");
                    keypress = "";
                }
                elsif (keypress == "r2") { 
                    setProp("/sim/model/efb/page", "Displays/Checklists_1.jpg");
                    setProp("/instrumentation/efb/page", "CHECKLISTS");
                    keypress = "";
                }
                elsif (keypress == "MENU") { 
                    setProp("/sim/model/efb/page", "Displays/Main_Menu.jpg");
                    setProp("/instrumentation/efb/page", "MENU");
                    keypress = "";
                }
            #__________________________________________________________________________________________
            # PERFORMANCE SECTION Parser --------------------------------------------------------------|
            #__________________________________________________________________________________________|
        } elsif (currentPage == "PERFORMANCE") {
            page.clearpage();

            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/Main_Menu.jpg");
                setProp("/instrumentation/efb/page", "MENU");
                keypress = "";
            }
        #__________________________________________________________________________________________
        # CHECKLISTS SECTION Parser ---------------------------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "CHECKLISTS") {
            page.clearpage();

            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/Main_Menu.jpg");
                setProp("/instrumentation/efb/page", "MENU");
                keypress = "";
            }
            if (keypress == "l2") { 
                setProp("/sim/model/efb/page", "Displays/Checklists_1_1.jpg");
                setProp("/instrumentation/efb/page", "CHECKLISTS");
                keypress = "";
            }
            if (keypress == "r2") { 
                setProp("/sim/model/efb/page", "Displays/Checklists_1_2.jpg");
                setProp("/instrumentation/efb/page", "CHECKLISTS");
                keypress = "";
            }
        #__________________________________________________________________________________________
        # IDENT SECTION Parser -------------------------------------------------------------------|
        #_________________________________________________________________________________________|
        } elsif (currentPage == "IDENT") {
            page.clearpage();

            IDENTl3 = getProp("/sim/description");
            IDENTl4 = substr(getProp("/sim/aircraft-operator"),0,3) ~ " / " ~ substr(getProp("/sim/multiplay/callsign"), 0,6); # this is to ensure that too long 'operator' and 'callsign' data can fit into the EFB display
            IDENTr4 = "VMEFB001"; # this is the EFb Seattle version's ID
            IDENTl5 = substr(getProp("environment/metar/data"),0,10);
            IDENTr5 = getProp("/instrumentation/clock/indicated-short-string") ~ "z";
            IDENTl8 = "AIRPORT MAP DATABASE    Seattle Data    APR1214-JUN3014"; # this is the DB version, which we will periodically update with better and new Charts

            setProp("/instrumentation/efb/display/IDENTline3-l", IDENTl3);
            setProp("/instrumentation/efb/display/IDENTline4-l", IDENTl4);
            setProp("/instrumentation/efb/display/IDENTline4-r", IDENTr4);
            setProp("/instrumentation/efb/display/IDENTline5-l", IDENTl5);
            setProp("/instrumentation/efb/display/IDENTline5-r", IDENTr5);
            setProp("/instrumentation/efb/display/IDENTline8-l", IDENTl8);

            setProp("/instrumentation/efb/display/input-helper", helper);
            setProp("/instrumentation/efb/keypress", keypress);

            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/Main_Menu.jpg");
                setProp("/instrumentation/efb/page", "MENU");
        
                IDENTl3 = "";
                IDENTl4 = "";
                IDENTr4 = "";
                IDENTl5 = "";
                IDENTr5 = "";
                IDENTl8 = "";
                setProp("/instrumentation/efb/display/IDENTline3-l", IDENTl3);
                setProp("/instrumentation/efb/display/IDENTline4-l", IDENTl4);
                setProp("/instrumentation/efb/display/IDENTline4-r", IDENTr4);
                setProp("/instrumentation/efb/display/IDENTline5-l", IDENTl5);
                setProp("/instrumentation/efb/display/IDENTline5-r", IDENTr5);
                setProp("/instrumentation/efb/display/IDENTline8-l", IDENTl8);

                keypress = "";
            }
        #__________________________________________________________________________________________
        #  SYSTEM SECTION Parser ------------------------------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "SYSTEMS") {
            page.clearpage();

            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/Main_Menu.jpg");
                setProp("/instrumentation/efb/page", "MENU");
                keypress = "";
            } 
        #__________________________________________________________________________________________
        #  CHART SECTION Parser -------------------------------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "CHARTS_KEYBOARD") {
            page.clearpage();
            page.update();
            page.charts_keyboard();

            if (getProp("/instrumentation/efb/chart/Searchable") == 0) { 
                setProp("/instrumentation/efb/Keyboard/CHART_Input_Line", "Enter ICAO");
            }

            # Get the ICAO to be searched for; "Virtual Keyboard" Keys Parser (see EFB_Chart_Keyboard.xml)

            if ((getProp("/instrumentation/efb/VKDRC_keypress") != "") and (getProp("/instrumentation/efb/VKDRC_keypress") != "SYMB") and (getProp("/instrumentation/efb/VKDRC_keypress") != "SHIFT")) {
                setProp("/instrumentation/efb/VK_Keyboard/Input_String", getProp("/instrumentation/efb/VK_Keyboard/Input_String") ~ getProp("/instrumentation/efb/VKDRC_keypress")); # Build the Input String  

                if (getProp("/instrumentation/efb/VKDRC_keypress") == "CLEAR") { 
            
                    setProp("/instrumentation/efb/VK_Keyboard/Input_String", ""); # CLEARs the whole Input Field and other stuff                                     
                }
                if (getProp("/instrumentation/efb/VKDRC_keypress") == "BKSP") { 
                    Temp_String = getProp("/instrumentation/efb/VK_Keyboard/Input_String"); # BACKSPACEs the Input Field
                    lenght = size(Temp_String) - 1;
                    setProp("/instrumentation/efb/VK_Keyboard/Input_String", substr(Temp_String, 0, lenght));
                    setProp("/instrumentation/efb/VKDRC_keypress", "");
                }
            }
            
            setProp("/instrumentation/efb/VKDRC_keypress", "");

            TMZ_String = getProp("/instrumentation/efb/VK_Keyboard/Input_String");
            TMZ_Input_Lenght = size(TMZ_String);

            if (TMZ_Input_Lenght <= 3)  { 
                setProp("/instrumentation/efb/Keyboard/CHART_Input_Line", getProp("/instrumentation/efb/VK_Keyboard/Input_String")); # Print it to the Keyboard Input Field

            } else { 
                setProp("/instrumentation/efb/chart/Searchable", 1);
                setProp("/instrumentation/efb/Keyboard/CHART_Input_Line", "Press [Search IDENT] to search for: " ~ getProp("/instrumentation/efb/VK_Keyboard/Input_String"));
            }
            if ((getProp("/instrumentation/efb/chart/Searchable") == 1) and (keypress == "l3")) { # do SEARCH CHART
                setProp("/instrumentation/efb/chart/icao", getProp("/instrumentation/efb/VK_Keyboard/Input_String"));
                page.charts_keyboard();
                setProp("/instrumentation/efb/VKDRC_keypress", "");
                setProp("/instrumentation/efb/chart/Searchable", 0);
                setProp("/instrumentation/efb/VK_Keyboard/Input_String", "");
            }
        
            if ((keypress == "PGDN") and (getProp("/instrumentation/efb/chart/PageList") < Chart_Pages)) {
                setProp("/instrumentation/efb/chart/SetPage", "INCREASE");
                setProp("/instrumentation/efb/chart/PageList", getProp("/instrumentation/efb/chart/PageList") + 1);
            }
            if ((keypress == "PGUP") and (getProp("/instrumentation/efb/chart/PageList") > 1)) {
                setProp("/instrumentation/efb/chart/SetPage", "INCREASE");
                setProp("/instrumentation/efb/chart/PageList", getProp("/instrumentation/efb/chart/PageList") + 1);
            }
            
            # Chart Selection - redirection to CHARTS...___________________________________________________________________________________

            if (keypress == "r1") { 
                setProp("/instrumentation/efb/chart/selected", getProp("/instrumentation/efb/chart/Selection_0"));
                keypress = "MENU";
            }
            if (keypress == "r2") { 
                setProp("/instrumentation/efb/chart/selected", getProp("/instrumentation/efb/chart/Selection_1"));
                keypress = "MENU";
            }
            if (keypress == "r3") { 
                setProp("/instrumentation/efb/chart/selected", getProp("/instrumentation/efb/chart/Selection_2"));
                keypress = "MENU";
            }
            # _____________________________________________________________________________________________________________________________

            if (keypress == "MENU") {
                # setProp("/sim/model/efb/page", "Displays/Blank_Test.jpg");
                setProp("/instrumentation/efb/chart/Status", "OFF");
                setProp("/instrumentation/efb/VK_Chart_Type", "APT"); # This starts and re-starts the Chart_Keyboard with active ICAO/APT Chart displayed ! (see B777 EFB Manual)
                setProp("/instrumentation/efb/VK_Keyboard/Input_String", "");
                setProp("/instrumentation/efb/page", "CHARTS");
                page.KCclearpage();
                page.KCupdate();
                keypress = "";
            }
            
        #__________________________________________________________________________________________
        #  INITIALIZE SECTION Parser --------------------------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "INITIALIZE") {
            page.clearpage();
            if (getProp("/instrumentation/efb/FlightStatus") == 0) {
                if (getProp("/autopilot/route-manager/departure/airport") != "")
                    OriginApt = getProp("/autopilot/route-manager/departure/airport");
                else
                    OriginApt = "NOT Set";
                if (getProp("/autopilot/route-manager/destination/airport") != "")
                    DestinationApt = getProp("/autopilot/route-manager/destination/airport");
                else
                    DestinationApt = "NOT Set";
                if ((OriginApt != "NOT Set") and (DestinationApt != "NOT Set")) {
                    OriginApt = getProp("/autopilot/route-manager/departure/airport");
                    DestinationApt = getProp("/autopilot/route-manager/destination/airport");
                    setProp("/sim/model/efb/page", "Displays/Initialize_1.jpg");
                    ClosingFlightFlag = 1; # Flight INITIABLE
                } else {
                    setProp("/sim/model/efb/page", "Displays/Initialize_2.jpg");
                    setProp("/instrumentation/efb/FlightStatus", 0);
                    ClosingFlightFlag = 0; # Flight NOT INITIABLE
                }
            } elsif ((getProp("/instrumentation/efb/FlightStatus") == 1) and (ClosingFlightFlag == 2)) {
                setProp("/instrumentation/efb/FlightStatus", 1); # Flight ACTIVE and CLOSABLE
                FlightStatus = 1;
                setProp("/sim/model/efb/page", "Displays/Initialize_3.jpg");
            }
        
            INITl4 = OriginApt;
            INITl5 = DestinationApt;
            setProp("/instrumentation/efb/display/INITline4-l", INITl4);
            setProp("/instrumentation/efb/display/INITline5-l", INITl5);
        
            if ((keypress == "r8") and (ClosingFlightFlag == 1)) { 
                setProp("/instrumentation/efb/FlightStatus", 1); # Flight INITIATED
                ClosingFlightFlag = 2; # Flight CLOSABLE
                keypress = "";
            }
            if ((keypress == "r8") and (FlightStatus == 1)) { 
                setProp("/instrumentation/efb/FlightStatus", 0); # Flight NOT ACTIVE
                setProp("/sim/model/efb/page", "Displays/Initialize_5.jpg");
                FlightStatus = 0;
                ClosingFlightFlag = 0; # Flight CLOSED
                keypress = "";
            }
            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/Main_Menu.jpg");
                setProp("/instrumentation/efb/page", "MENU");
                keypress = "";
                INITl4 = "";
                INITl5 = "";
                setProp("/instrumentation/efb/display/INITline4-l", INITl4);
                setProp("/instrumentation/efb/display/INITline5-l", INITl5);
                keypress = "";
            }
            
        #__________________________________________________________________________________________
        # PILOT UTILITIES SECTION Parser ----------------------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "UTILITIES") {
            page.clearpage();
            setProp("/instrumentation/efb/Keyboard/Input_String", "");
            setProp("/instrumentation/efb/Keyboard/Input_Line", "");
            setProp("/instrumentation/efb/Keyboard/Input_Line2", "");
            setProp("/instrumentation/efb/Keyboard/Input_Line3", "");
            setProp("/instrumentation/efb/VK_keypress", "");
            
            if (keypress == "l3") {
                setProp("/instrumentation/efb/page", "UTILITIES_DESC_RATE");
                setProp("/sim/model/efb/page", "Displays/PU_DRC.jpg");
                setProp("/instrumentation/efb/Keyboard/Input_String", "");
                setProp("/instrumentation/efb/VK_keypress", "");
                setProp("/instrumentation/efb/Input_Unit", "NO_INPUT");
                setProp("/instrumentation/efb/DRC_Initial_FL", "300");
                setProp("/instrumentation/efb/DRC_Target_FL", "30");
                setProp("/instrumentation/efb/DRC_Initial_GS", "250");
                setProp("/instrumentation/efb/DRC_Target_GS", "200");
                setProp("/instrumentation/efb/DRC_Distance", "50");
                setProp("/instrumentation/efb/DRC_Initial_FL_MEM", getProp("/instrumentation/efb/DRC_Initial_FL"));
                setProp("/instrumentation/efb/DRC_Target_FL_MEM", getProp("/instrumentation/efb/DRC_Target_FL"));
                setProp("/instrumentation/efb/DRC_Initial_GS_MEM", getProp("/instrumentation/efb/DRC_Initial_GS"));
                setProp("/instrumentation/efb/DRC_Target_GS_MEM", getProp("/instrumentation/efb/DRC_Target_GS"));
                setProp("/instrumentation/efb/DRC_Distance_MEM", getProp("/instrumentation/efb/DRC_Distance"));
                VK_Key = "";
                #setProp("/instrumentation/efb/VK_DRC_MarkerL", 1);
                Initial_FL = 300;
                Target_FL = 30;
                Initial_GS = 250;
                Target_GS = 200;
                DRC_Distance = 50;
                keypress = "";
            }
            if (keypress == "l4") {
                setProp("/instrumentation/efb/page", "GPS POSITION");
            }
            if (keypress == "r1") {
                setProp("/instrumentation/efb/page", "UTILITIES_CNV_SPD");
                setProp("/sim/model/efb/page", "Displays/PU_Cnv_Spd.jpg");
                setProp("/instrumentation/efb/VK_keypress", "");
                setProp("/instrumentation/efb/VK_IN_Marker", 1);
                setProp("/instrumentation/efb/VK_OUT_Marker", 1);
                VK_Input_Mem = 0;
                VK_Output_Mem = 0;
            }
            if (keypress == "r2") {
                setProp("/instrumentation/efb/page", "UTILITIES_CNV_LNG");
                setProp("/sim/model/efb/page", "Displays/PU_Cnv_Lng.jpg");
                setProp("/instrumentation/efb/VK_keypress", "");
                setProp("/instrumentation/efb/VK_IN_Marker", 1);
                setProp("/instrumentation/efb/VK_OUT_Marker", 1);
                VK_Input_Mem = 0;
                VK_Output_Mem = 0;
            }
            if (keypress == "r3") {
                setProp("/instrumentation/efb/page", "UTILITIES_CNV_WGT");
                setProp("/sim/model/efb/page", "Displays/PU_Cnv_Wgt.jpg");
                setProp("/instrumentation/efb/VK_keypress", "");
                setProp("/instrumentation/efb/VK_IN_Marker", 1);
                setProp("/instrumentation/efb/VK_OUT_Marker", 1);
                VK_Input_Mem = 0;
                VK_Output_Mem = 0;
                }
            if (keypress == "r4") {
                setProp("/instrumentation/efb/page", "UTILITIES_CNV_TMP");
                setProp("/sim/model/efb/page", "Displays/PU_Cnv_Tmp.jpg");
                setProp("/instrumentation/efb/VK_keypress", "");
                setProp("/instrumentation/efb/VK_IN_Marker", 1);
                setProp("/instrumentation/efb/VK_OUT_Marker", 1);
                VK_Input_Mem = 0;
                VK_Output_Mem = 0;
            }
            if (keypress == "r5") {
                setProp("/instrumentation/efb/page", "UTILITIES_CNV_VLM");
                setProp("/sim/model/efb/page", "Displays/PU_Cnv_Vlm.jpg");
                setProp("/instrumentation/efb/VK_keypress", "");
                setProp("/instrumentation/efb/VK_IN_Marker", 1);
                setProp("/instrumentation/efb/VK_OUT_Marker", 1);
                VK_Input_Mem = 0;
                VK_Output_Mem = 0;
            }
            if (keypress == "r6") {
                setProp("/instrumentation/efb/page", "UTILITIES_CNV_TMZ");
                setProp("/sim/model/efb/page", "Displays/PU_Cnv_Tmz.jpg");
                setProp("/instrumentation/efb/VK_keypress", "");
                setProp("/instrumentation/efb/VK_IN_Marker", 1);
                setProp("/instrumentation/efb/VK_OUT_Marker", 1);
                setProp("/instrumentation/efb/Keyboard/Input_String","");
                setProp("/instrumentation/efb/Keyboard/Input_HH", "--");
                setProp("/instrumentation/efb/Keyboard/Input_MM", "--");
                setProp("/instrumentation/efb/Keyboard/Output_HH", "");
                setProp("/instrumentation/efb/Keyboard/Output_MM", "");
                setProp("/instrumentation/efb/page/Cnv_Fact", "0");
                TMZ_String = "";
                TMZ_Index = 0;

                #Parsing for Month's names

                TMZ_DATE = substr(getProp("environment/metar/data"), 5, 5);

                if (substr(TMZ_DATE, 0, 2) == "01") TMZ_DATE = "JAN" ~ substr(TMZ_DATE, 2, 3);
                if (substr(TMZ_DATE, 0, 2) == "02") TMZ_DATE = "FEB" ~ substr(TMZ_DATE, 2, 3);
                if (substr(TMZ_DATE, 0, 2) == "03") TMZ_DATE = "MAR" ~ substr(TMZ_DATE, 2, 3);
                if (substr(TMZ_DATE, 0, 2) == "04") TMZ_DATE = "APR" ~ substr(TMZ_DATE, 2, 3);
                if (substr(TMZ_DATE, 0, 2) == "05") TMZ_DATE = "MAY" ~ substr(TMZ_DATE, 2, 3);
                if (substr(TMZ_DATE, 0, 2) == "06") TMZ_DATE = "JUN" ~ substr(TMZ_DATE, 2, 3);
                if (substr(TMZ_DATE, 0, 2) == "07") TMZ_DATE = "JUL" ~ substr(TMZ_DATE, 2, 3);
                if (substr(TMZ_DATE, 0, 2) == "08") TMZ_DATE = "AUG" ~ substr(TMZ_DATE, 2, 3);
                if (substr(TMZ_DATE, 0, 2) == "09") TMZ_DATE = "SEP" ~ substr(TMZ_DATE, 2, 3);
                if (substr(TMZ_DATE, 0, 2) == "10") TMZ_DATE = "OCT" ~ substr(TMZ_DATE, 2, 3);
                if (substr(TMZ_DATE, 0, 2) == "11") TMZ_DATE = "NOV" ~ substr(TMZ_DATE, 2, 3);
                if (substr(TMZ_DATE, 0, 2) == "12") TMZ_DATE = "DEC" ~ substr(TMZ_DATE, 2, 3);
            }
        
            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/Main_Menu.jpg");
                setProp("/instrumentation/efb/page", "MENU");
                keypress = "";
            }
            
        #__________________________________________________________________________________________
        # PILOT UTILITIES - GPS POSITION ----------------------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "GPS POSITION") {
            page.clearpage();
            page.gps();
        
            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/PU_1.jpg");
                setProp("/instrumentation/efb/page", "UTILITIES");
                keypress = "";
            }
            
        #__________________________________________________________________________________________
        # PILOT UTILITIES - SPEED CONVERSION Parser -----------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "UTILITIES_CNV_SPD") {
            Input_Unit = getProp("/instrumentation/efb/VK_IN_Marker");
            Output_Unit = getProp("/instrumentation/efb/VK_OUT_Marker");

            # Input keys Check

            if (getProp("/instrumentation/efb/VK_keypress") != "" or (Input_Unit != VK_Input_Mem) or (Output_Unit != VK_Output_Mem)) {
                # Input Field Parser
                if (getProp("/instrumentation/efb/VK_keypress") == "CLEAR") { 
                    setProp("/instrumentation/efb/Keyboard/Input_String", ""); # CLEARs the whole Input Field
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                if (getProp("/instrumentation/efb/VK_keypress") == "BKSP") { 
                    Temp_String = getProp("/instrumentation/efb/Keyboard/Input_String"); # BACKSPACEs the Input Field
                    lenght = size(Temp_String) - 1;
                    setProp("/instrumentation/efb/Keyboard/Input_String", substr(Temp_String, 0, lenght));
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                if (getProp("/instrumentation/efb/VK_keypress") == "CHNGS") { 
                    Temp_String = getProp("/instrumentation/efb/Keyboard/Input_String"); # CHANGEs SIGN to the Input Field
                    if (substr(Temp_String, 0, 1) == "-") { 
                        lenght = size(Temp_String) - 1;
                        Temp_String = substr(Temp_String, 1, lenght);
                    } else { 
                        Temp_String = "-" ~ Temp_String;
                    }
                    setProp("/instrumentation/efb/Keyboard/Input_String", Temp_String);
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                
                # Input Field Display

                setProp("/instrumentation/efb/Keyboard/Input_String", getProp("/instrumentation/efb/Keyboard/Input_String") ~ getProp("/instrumentation/efb/VK_keypress")); # Build the Input String
                setProp("/instrumentation/efb/Keyboard/Input_Line", getProp("/instrumentation/efb/Keyboard/Input_String"));                                                 # Print it to the Keyboard Input Field

                # ---------------> Conversion Table Selection
                # ---------------> Input Unit 1 () FT/M ---> Output Unit: FT/M, KM/H, KT, MTS, MPH
                if (Input_Unit == 1) {
                    if (Output_Unit == 1)    Cnv_Fact = 1;
                    elsif (Output_Unit == 2) Cnv_Fact = 0.018288;
                    elsif (Output_Unit == 3) Cnv_Fact = 0.0098747;
                    elsif (Output_Unit == 4) Cnv_Fact = 0.0050800;
                    elsif (Output_Unit == 5) {
                        if (getProp("/instrumentation/efb/VK_keypress") == "CLEAR") { 
                            setProp("/instrumentation/efb/Keyboard/Input_String", ""); # CLEARs the whole Input Field
                            setProp("/instrumentation/efb/VK_keypress", "");
                        }
                        elsif (getProp("/instrumentation/efb/VK_keypress") == "BKSP") { 
                            Temp_String = getProp("/instrumentation/efb/Keyboard/Input_String"); # BACKSPACEs the Input Field
                            lenght = size(Temp_String) - 1;
                            setProp("/instrumentation/efb/Keyboard/Input_String", substr(Temp_String, 0, lenght));
                            setProp("/instrumentation/efb/VK_keypress", "");
                        }
                        elsif (getProp("/instrumentation/efb/VK_keypress") == "CHNGS") { 
                            Temp_String = getProp("/instrumentation/efb/Keyboard/Input_String"); # CHANGEs SIGN to the Input Field
                            if (substr(Temp_String, 0, 1) == "-") { 
                                lenght = size(Temp_String) - 1;
                                Temp_String = substr(Temp_String, 1, lenght);
                            } else { 
                                Temp_String = "-" ~ Temp_String;
                            }
                            setProp("/instrumentation/efb/Keyboard/Input_String", Temp_String);
                            setProp("/instrumentation/efb/VK_keypress", "");
                        }
                        Cnv_Fact = 0.011364;
                    }
                }
                # ---------------> Input Unit 2 () KM/H ---> Output Unit: FT/M, KM/H, KT, MTS, MPH
                elsif (Input_Unit == 2) {
                    if (Output_Unit == 1)    Cnv_Fact = 54.68066;
                    elsif (Output_Unit == 2) Cnv_Fact = 1;
                    elsif (Output_Unit == 3) Cnv_Fact = 0.5399568;
                    elsif (Output_Unit == 4) Cnv_Fact = 0.2777778;
                    elsif (Output_Unit == 5) Cnv_Fact = 0.6213712;
                }
                # ---------------> Input Unit 3 () KT ---> Output Unit: FT/M, KM/H, KT, MTS, MPH
                elsif (Input_Unit == 3) {
                    if (Output_Unit == 1)    Cnv_Fact = 101.2686;
                    elsif (Output_Unit == 2) Cnv_Fact = 1.852000;
                    elsif (Output_Unit == 3) Cnv_Fact = 1;
                    elsif (Output_Unit == 4) Cnv_Fact = 0.5144444;
                    elsif (Output_Unit == 5) Cnv_Fact = 1.150779;
                }
                # ---------------> Input Unit 4 () MTS ---> Output Unit: FT/M, KM/H, KT, MTS, MPH
                elsif (Input_Unit == 4) {
                    if (Output_Unit == 1)    Cnv_Fact = 196.85;
                    elsif (Output_Unit == 2) Cnv_Fact = 3.6000;
                    elsif (Output_Unit == 3) Cnv_Fact = 1.9438;
                    elsif (Output_Unit == 4) Cnv_Fact = 1;
                    elsif (Output_Unit == 5) Cnv_Fact = 2.2369;
                }
                # ---------------> Input Unit 5 () MPH ---> Output Unit: FT/M, KM/H, KT, MTS, MPH
                elsif (Input_Unit == 5) {
                    if (Output_Unit == 1)    Cnv_Fact = 88.0002;
                    elsif (Output_Unit == 2) Cnv_Fact = 1.609344;
                    elsif (Output_Unit == 3) Cnv_Fact = 0.8689762;
                    elsif (Output_Unit == 4) Cnv_Fact = 0.4470400;
                    elsif (Output_Unit == 5) Cnv_Fact = 1;
                }
                VK_Input_Mem = Input_Unit;
                VK_Output_Mem = Output_Unit;
                
                # Output Field Display

                if ((getProp("/instrumentation/efb/Keyboard/Input_Line") == "") or (getProp("/instrumentation/efb/Keyboard/Input_Line") == ".") or (getProp("/instrumentation/efb/Keyboard/Input_Line") == "-")) { 
                    setProp("/instrumentation/efb/Keyboard/Output_Line", "");
                } else {
                    Output_Line = getProp("/instrumentation/efb/Keyboard/Input_Line");
                    Output_Line = Output_Line * Cnv_Fact; # Build the Input String
                    setProp("/instrumentation/efb/Keyboard/Output_Line", Output_Line); # Print it to the Keyboard Output Field
                }
                setProp("/instrumentation/efb/VK_keypress", "");
            }
            
            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/PU_1.jpg");
                setProp("/instrumentation/efb/page", "UTILITIES");
                setProp("/instrumentation/efb/Keyboard/Input_Line", "");
                setProp("/instrumentation/efb/Keyboard/Output_Line", "");
                keypress = "";
            }
        
        #__________________________________________________________________________________________
        # PILOT UTILITIES - LENGHT CONVERSION Parser ----------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "UTILITIES_CNV_LNG") {

            Input_Unit = getProp("/instrumentation/efb/VK_IN_Marker");
            Output_Unit = getProp("/instrumentation/efb/VK_OUT_Marker");

            # Input keys Check

            if (getProp("/instrumentation/efb/VK_keypress") != "" or (Input_Unit != VK_Input_Mem) or (Output_Unit != VK_Output_Mem)) {
                # Input Field Parser

                if (getProp("/instrumentation/efb/VK_keypress") == "CLEAR") {
                    setProp("/instrumentation/efb/Keyboard/Input_String", ""); # CLEARs the whole Input Field
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                elsif (getProp("/instrumentation/efb/VK_keypress") == "BKSP") { 
                    Temp_String = getProp("/instrumentation/efb/Keyboard/Input_String"); # BACKSPACEs the Input Field
                    lenght = size(Temp_String) - 1;
                    setProp("/instrumentation/efb/Keyboard/Input_String", substr(Temp_String, 0, lenght));
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                elsif (getProp("/instrumentation/efb/VK_keypress") == "CHNGS") { 
                    Temp_String = getProp("/instrumentation/efb/Keyboard/Input_String"); # CHANGEs SIGN to the Input Field
                    if (substr(Temp_String, 0, 1) == "-") { 
                        lenght = size(Temp_String) - 1;
                        Temp_String = substr(Temp_String, 1, lenght);
                    } else { 
                        Temp_String = "-" ~ Temp_String;
                    }
                    setProp("/instrumentation/efb/Keyboard/Input_String", Temp_String);
                    setProp("/instrumentation/efb/VK_keypress", "");
                }

                # Input Field Display

                setProp("/instrumentation/efb/Keyboard/Input_String", getProp("/instrumentation/efb/Keyboard/Input_String") ~ getProp("/instrumentation/efb/VK_keypress")); # Build the Input String
                setProp("/instrumentation/efb/Keyboard/Input_Line", getProp("/instrumentation/efb/Keyboard/Input_String")); # Print it to the Keyboard Input Field

                # ---------------> Conversion Table Selection

                # ---------------> Input Unit 1 () FEET ---> Output Unit: FEET, KILOMETERS, METERS, NAUTICAL MILES, STATUTE MILES
                if (Input_Unit == 1) {
                    if (Output_Unit == 1)    Cnv_Fact = 1;
                    elsif (Output_Unit == 2) Cnv_Fact = 0.0003048;
                    elsif (Output_Unit == 3) Cnv_Fact = 0.30480;
                    elsif (Output_Unit == 4) Cnv_Fact = 0.000164579; # International and US Nautical Miles, NOT UK Nautical Miles !!
                    elsif (Output_Unit == 5) Cnv_Fact = 0.000189394; # International Statute Miles
                }
                # ---------------> Input Unit 2 () KILOMETERS ---> Output Unit: FEET, KILOMETERS, METERS, NAUTICAL MILES, STATUTE MILES
                elsif (Input_Unit == 2) {
                    if (Output_Unit == 1) Cnv_Fact = 3280.83989501;
                    elsif (Output_Unit == 2) Cnv_Fact = 1;
                    elsif (Output_Unit == 3) Cnv_Fact = 1000;
                    elsif (Output_Unit == 4) Cnv_Fact = 0.539956803456; # International and US Nautical Miles, NOT UK Nautical Miles !!
                    elsif (Output_Unit == 5) Cnv_Fact = 0.621371192237; # International Statute Miles
                }
                # ---------------> Input Unit 3 () METERS ---> Output Unit: FEET, KILOMETERS, METERS, NAUTICAL MILES, STATUTE MILES
                elsif (Input_Unit == 3) {
                    if (Output_Unit == 1)    Cnv_Fact = 3.28083989501;
                    elsif (Output_Unit == 2) Cnv_Fact = 0.001;
                    elsif (Output_Unit == 3) Cnv_Fact = 1;
                    elsif (Output_Unit == 4) Cnv_Fact = 0.000539956803456; # International and US Nautical Miles, NOT UK Nautical Miles !!
                    elsif (Output_Unit == 5) Cnv_Fact = 0.000621371192237; # International Statute Miles
                }
                # ---------------> Input Unit 4 () NAUTICAL MILES (INTERNATIONAL!) ---> Output Unit: FEET, KILOMETERS, METERS, NAUTICAL MILES, STATUTE MILES
                elsif (Input_Unit == 4) {
                    if (Output_Unit == 1)    Cnv_Fact = 6076.11548556;
                    elsif (Output_Unit == 2) Cnv_Fact = 1.852;
                    elsif (Output_Unit == 3) Cnv_Fact = 1852;
                    elsif (Output_Unit == 4) Cnv_Fact = 1;
                    elsif (Output_Unit == 5) Cnv_Fact = 1.15077944802; # International Statute Miles
                }
                # ---------------> Input Unit 5 () STATUTE MILES ---> Output Unit: FEET, KILOMETERS, METERS, NAUTICAL MILES, STATUTE MILES
                elsif (Input_Unit == 5) {
                    if (Output_Unit == 1)    Cnv_Fact = 5280;
                    elsif (Output_Unit == 2) Cnv_Fact = 1.609344;
                    elsif (Output_Unit == 3) Cnv_Fact = 1609.344;
                    elsif (Output_Unit == 4) Cnv_Fact = 0.868976241901; # International and US Nautical Miles, NOT UK Nautical Miles !!
                    elsif (Output_Unit == 5) Cnv_Fact = 1;
                }
                VK_Input_Mem = Input_Unit;
                VK_Output_Mem = Output_Unit;
            
                # Output Field Display
                if ((getProp("/instrumentation/efb/Keyboard/Input_Line") == "") or (getProp("/instrumentation/efb/Keyboard/Input_Line") == ".") or (getProp("/instrumentation/efb/Keyboard/Input_Line") == "-")) { 
                    setProp("/instrumentation/efb/Keyboard/Output_Line", "");
                } else { 
                    Output_Line = getProp("/instrumentation/efb/Keyboard/Input_Line");
                    Output_Line = Output_Line * Cnv_Fact; # Build the Input String
                    setProp("/instrumentation/efb/Keyboard/Output_Line", Output_Line); # Print it to the Keyboard Output Field
                }
                setProp("/instrumentation/efb/VK_keypress", "");
            }

            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/PU_1.jpg");
                setProp("/instrumentation/efb/page", "UTILITIES");
                setProp("/instrumentation/efb/Keyboard/Input_Line", "");
                setProp("/instrumentation/efb/Keyboard/Output_Line", "");
                keypress = "";
            }
        
        #__________________________________________________________________________________________
        # PILOT UTILITIES - WEIGHT CONVERSION Parser ----------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "UTILITIES_CNV_WGT") {
            Input_Unit = getProp("/instrumentation/efb/VK_IN_Marker");
            Output_Unit = getProp("/instrumentation/efb/VK_OUT_Marker");
            
            # Input keys Check
            if (getProp("/instrumentation/efb/VK_keypress") != "" or (Input_Unit != VK_Input_Mem) or (Output_Unit != VK_Output_Mem)) {
                # Input Field Parser
                if (getProp("/instrumentation/efb/VK_keypress") == "CLEAR") { 
                    setProp("/instrumentation/efb/Keyboard/Input_String", ""); # CLEARs the whole Input Field
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                elsif (getProp("/instrumentation/efb/VK_keypress") == "BKSP") { 
                    Temp_String = getProp("/instrumentation/efb/Keyboard/Input_String"); # BACKSPACEs the Input Field
                    lenght = size(Temp_String) - 1;
                    setProp("/instrumentation/efb/Keyboard/Input_String", substr(Temp_String, 0, lenght));
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                elsif (getProp("/instrumentation/efb/VK_keypress") == "CHNGS") { 
                    Temp_String = getProp("/instrumentation/efb/Keyboard/Input_String"); # CHANGEs SIGN to the Input Field
                    if (substr(Temp_String, 0, 1) == "-") { 
                        lenght = size(Temp_String) - 1;
                        Temp_String = substr(Temp_String, 1, lenght);
                    } else { 
                        Temp_String = "-" ~ Temp_String;
                    }
                    setProp("/instrumentation/efb/Keyboard/Input_String", Temp_String);
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
        
                # Input Field Display
                setProp("/instrumentation/efb/Keyboard/Input_String", getProp("/instrumentation/efb/Keyboard/Input_String") ~ getProp("/instrumentation/efb/VK_keypress")); # Build the Input String
                setProp("/instrumentation/efb/Keyboard/Input_Line2", getProp("/instrumentation/efb/Keyboard/Input_String"));                                                # Print it to the Keyboard Input Field

                # ---------------> Conversion Table Selection
                # ---------------> Input Unit 1 () KILOGRAMS ---> Output Unit: KILOGRAMS, POUNDS
                if (Input_Unit == 1) {
                    if (Output_Unit == 1)    Cnv_Fact = 1;
                    elsif (Output_Unit == 2) Cnv_Fact = 2.20462262185;
                }
                # ---------------> Input Unit 2 () POUNDS ---> Output Unit: KILOGRAMS, POUNDS
                elsif (Input_Unit == 2) {
                    if (Output_Unit == 1)    Cnv_Fact = 0.45359237;
                    elsif (Output_Unit == 2) Cnv_Fact = 1;
                }
                VK_Input_Mem = Input_Unit;
                VK_Output_Mem = Output_Unit;
            
                # Output Field Display
                if ((getProp("/instrumentation/efb/Keyboard/Input_Line2") == "") or (getProp("/instrumentation/efb/Keyboard/Input_Line2") == ".") or (getProp("/instrumentation/efb/Keyboard/Input_Line2") == "-")) { 
                    setProp("/instrumentation/efb/Keyboard/Output_Line2", "");
                } else { 
                    Output_Line2 = getProp("/instrumentation/efb/Keyboard/Input_Line2");
                    Output_Line2 = Output_Line2 * Cnv_Fact; # Build the Input String
                    setProp("/instrumentation/efb/Keyboard/Output_Line2", Output_Line2); # Print it to the Keyboard Output Field
                }
                setProp("/instrumentation/efb/VK_keypress", "");
            }

            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/PU_1.jpg");
                setProp("/instrumentation/efb/page", "UTILITIES");
                setProp("/instrumentation/efb/Keyboard/Input_Line2", "");
                setProp("/instrumentation/efb/Keyboard/Output_Line2", "");
                keypress = "";
            }
       
        #__________________________________________________________________________________________
        # PILOT UTILITIES - TEMPERATURE CONVERSION Parser -----------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "UTILITIES_CNV_TMP") {

            Input_Unit = getProp("/instrumentation/efb/VK_IN_Marker");
            Output_Unit = getProp("/instrumentation/efb/VK_OUT_Marker");

            # Input keys Check
            if (getProp("/instrumentation/efb/VK_keypress") != "" or (Input_Unit != VK_Input_Mem) or (Output_Unit != VK_Output_Mem)) {
                # Input Field Parser
                if (getProp("/instrumentation/efb/VK_keypress") == "CLEAR") { 
                    setProp("/instrumentation/efb/Keyboard/Input_String", ""); # CLEARs the whole Input Field
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                elsif (getProp("/instrumentation/efb/VK_keypress") == "BKSP") { 
                    Temp_String = getProp("/instrumentation/efb/Keyboard/Input_String"); # BACKSPACEs the Input Field
                    lenght = size(Temp_String) - 1;
                    setProp("/instrumentation/efb/Keyboard/Input_String", substr(Temp_String, 0, lenght));
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                elsif (getProp("/instrumentation/efb/VK_keypress") == "CHNGS") { 
                    Temp_String = getProp("/instrumentation/efb/Keyboard/Input_String"); # CHANGEs SIGN to the Input Field
                    if (substr(Temp_String, 0, 1) == "-") { 
                        lenght = size(Temp_String) - 1;
                        Temp_String = substr(Temp_String, 1, lenght);
                    } else { 
                        Temp_String = "-" ~ Temp_String;
                    }
                    setProp("/instrumentation/efb/Keyboard/Input_String", Temp_String);
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                # Input Field Display
                setProp("/instrumentation/efb/Keyboard/Input_String", getProp("/instrumentation/efb/Keyboard/Input_String") ~ getProp("/instrumentation/efb/VK_keypress")); # Build the Input String
                setProp("/instrumentation/efb/Keyboard/Input_Line2", getProp("/instrumentation/efb/Keyboard/Input_String"));                                                # Print it to the Keyboard Input Field

                # Output Field Display
                if ((getProp("/instrumentation/efb/Keyboard/Input_Line2") == "") or (getProp("/instrumentation/efb/Keyboard/Input_Line2") == ".") or (getProp("/instrumentation/efb/Keyboard/Input_Line2") == "-")) { 
                    setProp("/instrumentation/efb/Keyboard/Output_Line2", "");
                } else { 
                    Output_Line2 = getProp("/instrumentation/efb/Keyboard/Input_Line2");
                    # ---------------> Conversion Table Selection
                    # ---------------> Input Unit 1 () CELSIUS (�C) ---> Output Unit: CELSIUS (�C), FAHRENHEIT (�F)
                    if (Input_Unit == 1) {
                        if (Output_Unit == 1)    Cnv_Fact = Output_Line2;
                        elsif (Output_Unit == 2) Cnv_Fact = (Output_Line2*1.8) + 32;
                    }
                    # ---------------> Input Unit 2 () FAHRENHEIT (�F) ---> Output Unit: CELSIUS (�C), FAHRENHEIT (�F)
                    elsif (Input_Unit == 2) {
                        if (Output_Unit == 1)    Cnv_Fact = (Output_Line2 - 32)/1.8;
                        elsif (Output_Unit == 2) Cnv_Fact = Output_Line2;
                    }
                    VK_Input_Mem = Input_Unit;
                    VK_Output_Mem = Output_Unit;

                    Output_Line2 = Cnv_Fact; # Build the Input String
                    setProp("/instrumentation/efb/Keyboard/Output_Line2", Output_Line2); # Print it to the Keyboard Output Field
                }
                setProp("/instrumentation/efb/VK_keypress", "");
            }
            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/PU_1.jpg");
                setProp("/instrumentation/efb/page", "UTILITIES");
                setProp("/instrumentation/efb/Keyboard/Input_Line2", "");
                setProp("/instrumentation/efb/Keyboard/Output_Line2", "");
                keypress = "";
            }
        #__________________________________________________________________________________________
        # PILOT UTILITIES - VOLUME CONVERSION Parser ----------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "UTILITIES_CNV_VLM") {

            Input_Unit = getProp("/instrumentation/efb/VK_IN_Marker");
            Output_Unit = getProp("/instrumentation/efb/VK_OUT_Marker");
            Output_TMZ_Line = "";
            # Input keys Check
            if (getProp("/instrumentation/efb/VK_keypress") != "" or (Input_Unit != VK_Input_Mem) or (Output_Unit != VK_Output_Mem)) {
                # Input Field Parser
                if (getProp("/instrumentation/efb/VK_keypress") == "CLEAR") { 
                    setProp("/instrumentation/efb/Keyboard/Input_String", ""); # CLEARs the whole Input Field
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                elsif (getProp("/instrumentation/efb/VK_keypress") == "BKSP") { 
                    Temp_String = getProp("/instrumentation/efb/Keyboard/Input_String"); # BACKSPACEs the Input Field
                    lenght = size(Temp_String) - 1;
                    setProp("/instrumentation/efb/Keyboard/Input_String", substr(Temp_String, 0, lenght));
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                elsif (getProp("/instrumentation/efb/VK_keypress") == "CHNGS") { 
                    Temp_String = getProp("/instrumentation/efb/Keyboard/Input_String"); # CHANGEs SIGN to the Input Field
                    if (substr(Temp_String, 0, 1) == "-") { 
                        lenght = size(Temp_String) - 1;
                        Temp_String = substr(Temp_String, 1, lenght);
                    } else { 
                        Temp_String = "-" ~ Temp_String;
                    }
                    setProp("/instrumentation/efb/Keyboard/Input_String", Temp_String);
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                # Input Field Display
                setProp("/instrumentation/efb/Keyboard/Input_String", getProp("/instrumentation/efb/Keyboard/Input_String") ~ getProp("/instrumentation/efb/VK_keypress")); # Build the Input String
                setProp("/instrumentation/efb/Keyboard/Input_Line3", getProp("/instrumentation/efb/Keyboard/Input_String")); # Print it to the Keyboard Input Field
            
                # ---------------> Conversion Table Selection
                # ---------------> Input Unit 1 () IMPERIAL GALLONS ---> Output Unit: IMPERIAL GALLONS, US GALLONS, LITERS
                if (Input_Unit == 1) {
                    if (Output_Unit == 1)    Cnv_Fact = 1;
                    elsif (Output_Unit == 2) Cnv_Fact = 1.20095;
                    elsif (Output_Unit == 3) Cnv_Fact = 4.54609;
                }
                # ---------------> Input Unit 2 () US GALLONS ---> Output Unit: IMPERIAL GALLONS, US GALLONS, LITERS
                elsif (Input_Unit == 2) {
                    if (Output_Unit == 1)    Cnv_Fact = 0.832674;
                    elsif (Output_Unit == 2) Cnv_Fact = 1;
                    elsif (Output_Unit == 3) Cnv_Fact = 3.78541;
                }
                # ---------------> Input Unit 3 () LITERS ---> Output Unit: IMPERIAL GALLONS, US GALLONS, LITERS
                elsif (Input_Unit == 3) {
                    if (Output_Unit == 1)    Cnv_Fact = 0.219969;
                    elsif (Output_Unit == 2) Cnv_Fact = 0.264172;
                    elsif (Output_Unit == 3) Cnv_Fact = 1;
                }
                VK_Input_Mem = Input_Unit;
                VK_Output_Mem = Output_Unit;
                # Output Field Display
                if ((getProp("/instrumentation/efb/Keyboard/Input_Line3") == "") or (getProp("/instrumentation/efb/Keyboard/Input_Line3") == ".") or (getProp("/instrumentation/efb/Keyboard/Input_Line3") == "-")) { 
                    setProp("/instrumentation/efb/Keyboard/Output_Line3", "");
                } else { 
                    Output_Line2 = getProp("/instrumentation/efb/Keyboard/Input_Line3");
                    Output_Line2 = Output_Line2 * Cnv_Fact; # Build the Input String
                    setProp("/instrumentation/efb/Keyboard/Output_Line3", Output_Line2); # Print it to the Keyboard Output Field
                }
                setProp("/instrumentation/efb/VK_keypress", "");
            }
            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/PU_1.jpg");
                setProp("/instrumentation/efb/page", "UTILITIES");
                setProp("/instrumentation/efb/Keyboard/Input_Line3", "");
                setProp("/instrumentation/efb/Keyboard/Output_Line3", "");
                keypress = "";
            }
        
        #__________________________________________________________________________________________
        # PILOT UTILITIES - TIME ZONE CONVERSION Parser -------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "UTILITIES_CNV_TMZ") {
            
            TMZ_TIME = getProp("/instrumentation/clock/indicated-short-string") ~ "z";
            setProp("/instrumentation/efb/Keyboard/Current_DT_Line", TMZ_DATE ~ " " ~ TMZ_TIME);                                                                                                                # Print Current Date & Time
            setProp("/instrumentation/efb/Keyboard/Converted_Name_Line", "Use Keyboard to Input Time (HHMM)");
            # Input keys Check

            if (getProp("/instrumentation/efb/VK_keypress") != "") {
                # Input Field Parser
                if (getProp("/instrumentation/efb/VK_keypress") == "CLEAR") { 
                    setProp("/instrumentation/efb/Keyboard/Input_String", ""); # CLEARs the whole Input Field and other stuff
                    setProp("/instrumentation/efb/Keyboard/Input_HH_Line", "--");
                    setProp("/instrumentation/efb/Keyboard/Input_MM_Line", "--");
                    setProp("/instrumentation/efb/Keyboard/Input_Zulu_Line", "--:--");
                    setProp("/instrumentation/efb/Keyboard/Output_HH", "");
                    setProp("/instrumentation/efb/Keyboard/Output_MM", "");
                    setProp("/instrumentation/efb/Keyboard/Converted_ID_Line", "");
                    setProp("/instrumentation/efb/Keyboard/Converted_Name_Line", "Use Keyboard to Input Time (HHMM)");
                    setProp("/instrumentation/efb/Keyboard/Output_TMZ_Line", "--:--");
                    setProp("/instrumentation/efb/Keyboard/Cnv_Fact", "");
                    TMZ_index = 0;
                    Output_TMz_Line = "--:--";
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                elsif (getProp("/instrumentation/efb/VK_keypress") == "BKSP") { 
                    Temp_String = getProp("/instrumentation/efb/Keyboard/Input_String"); # BACKSPACEs the Input Field
                    lenght = size(Temp_String) - 1;
                    setProp("/instrumentation/efb/Keyboard/Input_String", substr(Temp_String, 0, lenght));
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                elsif (getProp("/instrumentation/efb/VK_keypress") == "CHNGS") { 
                    setProp("/instrumentation/efb/Keyboard/Input_String", getProp("/instrumentation/efb/Keyboard/Input_String"));                     # Bypasses CHANGEs SIGN
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                
                # Input Field Display
                TMZ_String = getProp("/instrumentation/efb/Keyboard/Input_String");
                TMZ_Input_Lenght = size(TMZ_String);

                if (TMZ_Input_Lenght <= 3)  {
                    setProp("/instrumentation/efb/Keyboard/Input_String", getProp("/instrumentation/efb/Keyboard/Input_String") ~ getProp("/instrumentation/efb/VK_keypress"));                                 # Build the Input String

                    if (TMZ_Input_Lenght > 1) { 
                        setProp("/instrumentation/efb/Keyboard/Input_HH_Line", substr(getProp("/instrumentation/efb/Keyboard/Input_String"), 0, 2));                                    # IF 3rd digit, Print HH to the Keyboard Input Field
                        setProp("/instrumentation/efb/Keyboard/Output_HH", substr(getProp("/instrumentation/efb/Keyboard/Input_String"), 0, 2));                                                                    # Copy HH to Output_HH
                        setProp("/instrumentation/efb/Keyboard/Input_MM_Line", substr(getProp("/instrumentation/efb/Keyboard/Input_String"), 2, TMZ_Input_Lenght));                                                 # IF 3rd digit, Print MM to the Keyboard Input Field
                        setProp("/instrumentation/efb/Keyboard/Output_MM", substr(getProp("/instrumentation/efb/Keyboard/Input_String"), 2, TMZ_Input_Lenght));                                                     # Copy MM to Output_MM
                        setProp("/instrumentation/efb/Keyboard/Input_Zulu_Line", (getProp("/instrumentation/efb/Keyboard/Input_HH_Line") ~ ":" ~ getProp("/instrumentation/efb/Keyboard/Input_MM_Line")));          # IF 3rd digit, Print "zulu Time" to the Keyboard Output "zulu to be converted" Field (see EFB Manual)
                    } else { 
                        setProp("/instrumentation/efb/Keyboard/Input_HH_Line", getProp("/instrumentation/efb/Keyboard/Input_String"));                                                                                                                         # Print Blank HH to the Keyboard Input Field
                        setProp("/instrumentation/efb/Keyboard/Input_MM_Line", "--");                                                                                                                      # Print Blank HH to the Keyboard Input Field
                        setProp("/instrumentation/efb/Keyboard/Input_Zulu_Line", getProp("/instrumentation/efb/Keyboard/Input_HH_Line") ~ ":" ~ getProp("/instrumentation/efb/Keyboard/Input_MM_Line"));   # Print Blank "zulu Time" to the Keyboard Output "zulu to be converted" Field (see EFB Manual)                                                                                                                          # Print "" MM to the Keyboard Input Field
                        setProp("/instrumentation/efb/Keyboard/Output_HH", "");
                        setProp("/instrumentation/efb/Keyboard/Output_MM", "");
                    }

                } # an 'else' clause should be set here, giving the Pilot an audible 'WARNING Beep', because we cannot accept more than 4 chars for Time Input!
            }
            setProp("/instrumentation/efb/VK_keypress", ""); # reset the VK keys to ""

            # ---------------> Output Unit: Input Time Conversion --> TIME ZONES Time

            if (TMZ_Input_Lenght > 1) { 
                # TMZ_Index = TMZ_Index;
                if ((keypress == "r3") and (TMZ_Index > 0)) { 
                    TMZ_Index = TMZ_Index - 1;
                    keypress = "";
                } # an 'else' clause should be set here, giving the Pilot an audible 'WARNING Beep', because he reached Bottom End of DB File!
                if ((keypress == "r4") and (TMZ_Index < TMZ_DB_Size)) { 
                    TMZ_Index = TMZ_Index + 1;
                    keypress = "";
                } # an 'else' clause should be set here, giving the Pilot an audible 'WARNING Beep', because he reached Top End of DB File!
                #setProp("/instrumentation/efb/Keyboard/Output_HH", "12");
                #setProp("/instrumentation/efb/Keyboard/Output_MM", "54");
                if (keypress == "r5") { setProp("/instrumentation/efb/Keyboard/Cnv_Fact", "0"); # RESET ALL
                    setProp("/instrumentation/efb/Keyboard/Input_String", "");
                    TMZ_String = "";
                    TMZ_Index = 0;
                    TMZ_Input_Lenght = 0;
                    setProp("/instrumentation/efb/Keyboard/Current_DT_Line", "");
                    setProp("/instrumentation/efb/Keyboard/Input_HH_Line", "");
                    setProp("/instrumentation/efb/Keyboard/Input_MM_Line", "");
                    setProp("/instrumentation/efb/Keyboard/Input_Zulu_Line", "");
                    setProp("/instrumentation/efb/Keyboard/Output_HH", "0");
                    setProp("/instrumentation/efb/Keyboard/Output_MM", "0");
                    setProp("/instrumentation/efb/Keyboard/Converted_ID_Line", "");
                    setProp("/instrumentation/efb/Keyboard/Converted_Name_Line", "Use Keyboard to Input Time (HHMM)");
                    setProp("/instrumentation/efb/Keyboard/Output_TMZ_Line", "");
                    setProp("/instrumentation/efb/Keyboard/Cnv_Fact", "0");
                    TMZ_index = 0;
                    Output_TMz_Line = "--:--";
                }
                if (getProp("/instrumentation/efb/Keyboard/Output_HH") != "0") {
                    TMZ_Converted_ID = getProp("/instrumentation/efb/TimeZonesDB/IDX[" ~ TMZ_Index ~ "]/ID");
                    TMZ_Converted_Name = getProp("/instrumentation/efb/TimeZonesDB/IDX[" ~ TMZ_Index ~ "]/Name");
                    TMZ_zulu_HH = getProp("/instrumentation/efb/Keyboard/Output_HH");
                    TMZ_Converted_OffSet = getProp("/instrumentation/efb/TimeZonesDB/IDX[" ~ TMZ_Index ~ "]/OffSet");
                    setProp("/instrumentation/efb/Keyboard/Converted_ID_Line", TMZ_Converted_ID); # Print Time Zone ID to the Keyboard Converted Time Field
                    setProp("/instrumentation/efb/Keyboard/Converted_Name_Line", TMZ_Converted_Name); # Print Time Zone Name to the Keyboard Converted Time Field
                    setProp("/instrumentation/efb/Keyboard/Cnv_Fact", (TMZ_zulu_HH + TMZ_Converted_OffSet));
                } else { 
                    setProp("/instrumentation/efb/Keyboard/Cnv_Fact", 0);
                }
                # HH Corrections for Time Zone's Conversion Factors
                if (getProp("/instrumentation/efb/Keyboard/Cnv_Fact") > 9) { 
                    setProp("/instrumentation/efb/Keyboard/Cnv_Fact", getProp("/instrumentation/efb/Keyboard/Cnv_Fact"));                                                                              # Print HH Converted Time to the Keyboard Converted Time Field
                } elsif (((getProp("/instrumentation/efb/Keyboard/Cnv_Fact") >= 0) and (getProp("/instrumentation/efb/Keyboard/Cnv_Fact") <= 9))) { 
                    setProp("/instrumentation/efb/Keyboard/Cnv_Fact", "0" ~ getProp("/instrumentation/efb/Keyboard/Cnv_Fact")); # Print H Converted Time to the Keyboard Converted Time Field
                } elsif (getProp("/instrumentation/efb/Keyboard/Cnv_Fact") < 0) { 
                    setProp("/instrumentation/efb/Keyboard/Cnv_Fact", (24 + getProp("/instrumentation/efb/Keyboard/Cnv_Fact")));                                                                  # Print (24 - HH) Converted Time to the Keyboard Converted Time Field
                }
            } else { 
                setProp("/instrumentation/efb/Keyboard/Cnv_Fact", "--");
            }
            # Output Converted Time Field Display
            if ((getProp("/instrumentation/efb/Keyboard/Input_HH_Line") == "") or (getProp("/instrumentation/efb/Keyboard/Input_HH_Line") == ".") or (getProp("/instrumentation/efb/Keyboard/Input_HH_Line") == "--")) { 
                setProp("/instrumentation/efb/Keyboard/Output_TMZ_Line", "");
                Output_TMz_Line = "--:--";
            } else { 
                Output_TMZ_Line = getProp("/instrumentation/efb/Keyboard/Cnv_Fact");
                Output_TMZ_Line = substr(Output_TMZ_Line, 0, 2) ~ ":" ~ getProp("/instrumentation/efb/Keyboard/Output_MM");# Build the Output String
            }
            setProp("/instrumentation/efb/Keyboard/Output_TMZ_Line", Output_TMZ_Line); # Print it to the Keyboard Output Converted Time Field
            
            if (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/PU_1.jpg");
                setProp("/instrumentation/efb/page", "UTILITIES");
                setProp("/instrumentation/efb/Keyboard/Current_DT_Line", "");
                setProp("/instrumentation/efb/Keyboard/Input_HH_Line", "");
                setProp("/instrumentation/efb/Keyboard/Input_MM_Line", "");
                setProp("/instrumentation/efb/Keyboard/Input_Zulu_Line", "");
                setProp("/instrumentation/efb/Keyboard/Cnv_Fact", "");
                setProp("/instrumentation/efb/Keyboard/Output_HH", "");
                setProp("/instrumentation/efb/Keyboard/Output_MM", "");
                setProp("/instrumentation/efb/Keyboard/Converted_ID_Line", "");
                setProp("/instrumentation/efb/Keyboard/Converted_Name_Line", "");
                setProp("/instrumentation/efb/Keyboard/Output_TMZ_Line", "");
                setProp("/instrumentation/efb/Keyboard/Input_String", "");

                keypress = "";
            }  
        
        #__________________________________________________________________________________________
        # PILOT UTILITIES - DESCENT RATE Calculator ----------------------------------------------|
        #_________________________________________________________________________________________|
        } elsif (currentPage == "UTILITIES_DESC_RATE") {
            
            page.clearpage();
            # Input keys Check
            if (keypress == "l2") { 
                setProp("/instrumentation/efb/Input_Unit", "Initial_FL");
                setProp("/instrumentation/efb/Keyboard/Input_String","");
                setProp("/instrumentation/efb/DRC_Initial_FL_MEM", getProp("/instrumentation/efb/DRC_Initial_FL"));
                setProp("/instrumentation/efb/VK_DRC_MarkerL", 1);
                setProp("/instrumentation/efb/VK_DRC_MarkerR", 0);
            }
            elsif (keypress == "r2") { 
                setProp("/instrumentation/efb/Input_Unit", "Target_FL");
                setProp("/instrumentation/efb/Keyboard/Input_String","");
                setProp("/instrumentation/efb/DRC_Target_FL_MEM", getProp("/instrumentation/efb/DRC_Target_FL"));
                setProp("/instrumentation/efb/VK_DRC_MarkerR", 1);
                setProp("/instrumentation/efb/VK_DRC_MarkerL", 0);
            }
            elsif (keypress == "l3") { 
                setProp("/instrumentation/efb/Input_Unit", "Initial_GS");
                setProp("/instrumentation/efb/Keyboard/Input_String","");
                setProp("/instrumentation/efb/DRC_Initial_GS_MEM", getProp("/instrumentation/efb/DRC_Initial_GS"));
                setProp("/instrumentation/efb/VK_DRC_MarkerL", 2);
                setProp("/instrumentation/efb/VK_DRC_MarkerR", 0);
            }
            elsif (keypress == "r3") { 
                setProp("/instrumentation/efb/Input_Unit", "Target_GS");
                setProp("/instrumentation/efb/Keyboard/Input_String","");
                setProp("/instrumentation/efb/DRC_Target_GS_MEM", getProp("/instrumentation/efb/DRC_Target_GS"));
                setProp("/instrumentation/efb/VK_DRC_MarkerR", 2);
                setProp("/instrumentation/efb/VK_DRC_MarkerL", 0);
            }
            elsif (keypress == "l4") { 
                setProp("/instrumentation/efb/Input_Unit", "Distance");
                setProp("/instrumentation/efb/Keyboard/Input_String","");
                setProp("/instrumentation/efb/DRC_Distance_MEM", getProp("/instrumentation/efb/DRC_Distance"));
                setProp("/instrumentation/efb/VK_DRC_MarkerL", 3);
                setProp("/instrumentation/efb/VK_DRC_MarkerR", 0);
            }
            
            # Input keys Parser
            if ((getProp("/instrumentation/efb/VK_keypress") != "") and (getProp("/instrumentation/efb/VK_keypress") != ".") and (getProp("/instrumentation/efb/VK_keypress") != "CHNGS") and (getProp("/instrumentation/efb/Input_Unit") != "NO_INPUT")) {
                if (getProp("/instrumentation/efb/VK_keypress") == "CLEAR") { 
                    setProp("/instrumentation/efb/Keyboard/Input_String", ""); # CLEARs the whole Input Field
                    VK_Key = "VOID";
                    setProp("/instrumentation/efb/VK_keypress", "");
                }
                elsif (getProp("/instrumentation/efb/VK_keypress") == "BKSP") {
                    Temp_String = getProp("/instrumentation/efb/Keyboard/Input_String"); # BACKSPACEs the Input Field
                    lenght = size(Temp_String) - 1;
                    if (lenght >= 1) {
                        setProp("/instrumentation/efb/Keyboard/Input_String", substr(Temp_String, 0, lenght));
                        setProp("/instrumentation/efb/VK_keypress", "");
                    } elsif (lenght <= 0) { 
                        setProp("/instrumentation/efb/Keyboard/Input_String", ""); # CLEARs the whole Input Field
                        VK_Key = "VOID";
                    }
                }
                DRC_String = getProp("/instrumentation/efb/Keyboard/Input_String");
                DRC_Input_Lenght = size(DRC_String);
            
                if (DRC_Input_Lenght < 3) {
                    if (VK_Key != "VOID") { 
                        setProp("/instrumentation/efb/Keyboard/Input_String", getProp("/instrumentation/efb/Keyboard/Input_String") ~ getProp("/instrumentation/efb/VK_keypress"));                 # Build the Input String...
                    }
                    if ((getProp("/instrumentation/efb/Input_Unit") == "Initial_FL") and (VK_Key != "VOID")) { 
                        setProp("/instrumentation/efb/DRC_Initial_FL", getProp("/instrumentation/efb/Keyboard/Input_String"));   # ...For Initial FL
                    } elsif ((getProp("/instrumentation/efb/Input_Unit") == "Initial_FL") and (VK_Key == "VOID")) { 
                        setProp("/instrumentation/efb/display/DRC_l3", "CLEAR");
                        setProp("/instrumentation/efb/DRC_Initial_FL", getProp("/instrumentation/efb/DRC_Initial_FL_MEM"));
                    }
                    if ((getProp("/instrumentation/efb/Input_Unit") == "Target_FL") and (VK_Key != "VOID")) { 
                        setProp("/instrumentation/efb/DRC_Target_FL", getProp("/instrumentation/efb/Keyboard/Input_String"));     # ...For Target FL
                    } elsif ((getProp("/instrumentation/efb/Input_Unit") == "Target_FL") and (VK_Key == "VOID")) { 
                        setProp("/instrumentation/efb/display/DRC_r3", "CLEAR");
                        setProp("/instrumentation/efb/DRC_Target_FL", getProp("/instrumentation/efb/DRC_Target_FL_MEM"));
                    }
                    if ((getProp("/instrumentation/efb/Input_Unit") == "Initial_GS") and (VK_Key != "VOID")) { 
                        setProp("/instrumentation/efb/DRC_Initial_GS", getProp("/instrumentation/efb/Keyboard/Input_String"));   # ...For Initial GS
                    } elsif ((getProp("/instrumentation/efb/Input_Unit") == "Initial_GS") and (VK_Key == "VOID")) { 
                        setProp("/instrumentation/efb/display/DRC_l5", "CLEAR");
                        setProp("/instrumentation/efb/DRC_Initial_GS", getProp("/instrumentation/efb/DRC_Initial_GS_MEM"));
                    }
                    if ((getProp("/instrumentation/efb/Input_Unit") == "Target_GS") and (VK_Key != "VOID")) { 
                        setProp("/instrumentation/efb/DRC_Target_GS", getProp("/instrumentation/efb/Keyboard/Input_String"));     # ...For Target GS
                    } elsif ((getProp("/instrumentation/efb/Input_Unit") == "Target_GS") and (VK_Key == "VOID")) { 
                        setProp("/instrumentation/efb/display/DRC_r5", "CLEAR");
                        setProp("/instrumentation/efb/DRC_Target_GS", getProp("/instrumentation/efb/DRC_Target_GS_MEM"));
                    }
                    if ((getProp("/instrumentation/efb/Input_Unit") == "Distance")  and (VK_Key != "VOID")) { 
                        setProp("/instrumentation/efb/DRC_Distance", getProp("/instrumentation/efb/Keyboard/Input_String"));      # ...For Distance
                    } elsif ((getProp("/instrumentation/efb/Input_Unit") == "Distance")  and (VK_Key == "VOID")) { 
                        setProp("/instrumentation/efb/display/DRC_l7", "CLEAR");
                        setProp("/instrumentation/efb/DRC_Distance", getProp("/instrumentation/efb/DRC_Distance_MEM"));
                    }
                }
            }
            setProp("/instrumentation/efb/VK_keypress", ""); # reset the VK keys to ""
            VK_Key = "";
            
            # Transfers Values to DRC variables as numbers
            Initial_FL = num(getProp("/instrumentation/efb/DRC_Initial_FL"));
            Target_FL = num(getProp("/instrumentation/efb/DRC_Target_FL"));
            Initial_GS = num(getProp("/instrumentation/efb/DRC_Initial_GS"));
            Target_GS = num(getProp("/instrumentation/efb/DRC_Target_GS"));
            DRC_Distance = num(getProp("/instrumentation/efb/DRC_Distance"));
            
            # Calculates Formulas
            # Formula: [Angle of Descent (ft/NM) = ALT Gradient/Distance] |||| [SPD Factor (ft/min) = SPD Gradient/60] |||| [Target Descent Rate (Fpm) = Angle of Descent * SPD Factor]

            var DRC_Angle = (Target_FL - Initial_FL)/DRC_Distance;
            var DRC_AvgSpeed = (Initial_GS + Target_GS)/2;
            var DRC_Fact = DRC_AvgSpeed/60;
            var DRC_Output = DRC_Angle * DRC_Fact*100;
            var DRC_Time = (Target_FL - Initial_FL)/(DRC_Output/100);
            
            # Prepares Output Strings
            DRC_l3 = substr(getProp("/instrumentation/efb/DRC_Initial_FL"), 0, 3);
            CUT = split(".", DRC_l3);
            DRC_l3 = CUT[0];

            DRC_r3 = substr(getProp("/instrumentation/efb/DRC_Target_FL"), 0, 3);
            CUT = split(".", DRC_r3);
            DRC_r3 = CUT[0];

            DRC_l5 = substr(getProp("/instrumentation/efb/DRC_Initial_GS"), 0, 3);
            CUT = split(".", DRC_l5);
            DRC_l5 = CUT[0];

            DRC_r5 = substr(getProp("/instrumentation/efb/DRC_Target_GS"), 0, 3);
            CUT = split(".", DRC_r5);
            DRC_r5 = CUT[0];

            DRC_l7 = substr(getProp("/instrumentation/efb/DRC_Distance"), 0, 3);
            CUT = split(".", DRC_l7);
            DRC_l7 = CUT[0];

            setProp("/instrumentation/efb/DRC_Output", "100");
            setProp("/instrumentation/efb/DRC_Output", DRC_Output);
            DRC_r8 = substr(getProp("/instrumentation/efb/DRC_Output"), 0, 8);
            CUT = split(".", DRC_r8);
            DRC_r8 = CUT[0];

            setProp("/instrumentation/efb/DRC_Angle", "100");
            setProp("/instrumentation/efb/DRC_Angle", DRC_Angle);
            DRC_r9 = substr(getProp("/instrumentation/efb/DRC_Angle"), 0, 5);
            # CUT = split(".", DRC_r9);
            # DRC_r9 = CUT[0];

            setProp("/instrumentation/efb/DRC_Time", "100");
            setProp("/instrumentation/efb/DRC_Time", DRC_Time);
            DRC_r10 = substr(getProp("/instrumentation/efb/DRC_Time"), 0, 5);
            #CUT = split(".", DRC_r10);
            #DRC_r10 = CUT[0];

            
            page.DRCupdate();   # Displays Output

            if (keypress == "MENU") { 
                page.DRCclearpage();
                page.DRCupdate();
                setProp("/sim/model/efb/page", "Displays/PU_1.jpg");
                setProp("/instrumentation/efb/VK_DRC_MarkerR", 0);
                setProp("/instrumentation/efb/VK_DRC_MarkerL", 0);
                setProp("/instrumentation/efb/page", "UTILITIES");
                keypress = "";
            }
        #__________________________________________________________________________________________
        # PILOT UTILITIES - NORMAL PROCEDURES MANUAL ----------------------------------------------|
        #__________________________________________________________________________________________|
        } elsif (currentPage == "NORM PROC MANUAL") {
            page.clearpage();

            if (keypress == "PGUP") { 
                setProp("/instrumentation/efb/manual-page", getProp("/instrumentation/efb/manual-page") - 0.20);
                keypress = "";
            }
            elsif (keypress == "ScrollUP") { 
                setProp("/instrumentation/efb/manual-page", getProp("/instrumentation/efb/manual-page") - 0.02);
                keypress = "";
            }
            elsif (keypress == "ScrollDN") { 
                setProp("/instrumentation/efb/manual-page", getProp("/instrumentation/efb/manual-page") + 0.02);
                keypress = "";
            }
            elsif (keypress == "PGDN") { 
                setProp("/instrumentation/efb/manual-page", getProp("/instrumentation/efb/manual-page") + 0.20);
                keypress = "";
            }
            elsif (keypress == "MENU") { 
                setProp("/sim/model/efb/page", "Displays/Documents_1.jpg");
                setProp("/instrumentation/efb/page", "DOCUMENTS");
                keypress = "";
            }
        }
        #___________________________________________________________________________________________
        #_______________________________END of Parsers & SubParsers_________________________________|
        page.update();
        if ((getProp("/instrumentation/efb/page") == "Airport Charts") or (getProp("/instrumentation/efb/page") == "Airport Diagram") or (getProp("/instrumentation/efb/page") == "NORM PROC MANUAL")) setProp("/instrumentation/efb/text-color", 0);
        else setProp("/instrumentation/efb/text-color", 1);
    },
    reset : func {
        if(!me.efbTimer)
            me.efbTimer = maketimer(me.UPDATE_INTERVAL, func me.update());
        else
            me.efbTimer.stop();
        me.loopid += 1;
        #//me._loop_(me.loopid);
        me.efbTimer.start();
    },
    #//_loop_ : func() {
    #//    id == me.loopid or return;
    #//    me.update();
    #//}
};

var toggle = func(property) {
    if (getProp(property) == 1)
        setProp(property, 0);
    else
        setProp(property, 1);
}
setlistener("sim/signals/fdm-initialized", func {efb.init();});
