# Road check and auto pilot(??) by ValKmjolnir

var position_change = func(position_val,value){
    if(position_val+value>180)
        position_val += value-360;
    else if(position_val+value<-180)
        position_val += value+360;
    else
        position_val += value;
    return position_val;
}
var road_check_func = func(){
    
    var lat = props.getNode("/position/latitude-deg",1).getValue();
    var lon = props.getNode("/position/longitude-deg",1).getValue();
    var position_info = geodinfo(lat,lon);
    var position_names = position_info[1].names;
    # the friction_factor of freeway runway and road is 1

    if((position_names[0]=="Freeway") or (position_names[0]=="Road"))
    {
        var car_heading = 0;
        var lat_change  = 0;
        var lon_change  = 0;
        var left_range  = 0;
        var right_range = 0;
        
        for(var i=0;i>-0.00005;i-=0.000001)
        {
            car_heading = props.getNode("/orientation/heading-deg",1).getValue();
            lat_change  = math.sin(math.pi*car_heading/180);
            lon_change  = -math.cos(math.pi*car_heading/180);
            lat = props.getNode("/position/latitude-deg",1).getValue()+0.0001*math.cos(math.pi*car_heading/180);
            lon = props.getNode("/position/longitude-deg",1).getValue()+0.0001*math.sin(math.pi*car_heading/180);
            var other_position_info = geodinfo(position_change(lat,i*lat_change),position_change(lon,i*lon_change));
            var other_names = other_position_info[1].names;
            if((other_names[0]=="Freeway") or (other_names[0]=="Road"))
                right_range += 1;
            else
                break;
        }
        for(var i=0;i<0.00005;i+=0.000001)
        {
            car_heading = props.getNode("/orientation/heading-deg",1).getValue();
            lat_change  = math.sin(math.pi*car_heading/180);
            lon_change  = -math.cos(math.pi*car_heading/180);
            lat = props.getNode("/position/latitude-deg",1).getValue()+0.0001*math.cos(math.pi*car_heading/180);
            lon = props.getNode("/position/longitude-deg",1).getValue()+0.0001*math.sin(math.pi*car_heading/180);
            var other_position_info = geodinfo(position_change(lat,i*lat_change),position_change(lon,i*lon_change));
            var other_names = other_position_info[1].names;
            if((other_names[0]=="Freeway") or (other_names[0]=="Road"))
                left_range+=1;
            else
                break;
        }
        #if(left_range>right_range)
        #{
        #    setprop("/controls/flight/rudder",-(right_range-left_range)*(right_range-left_range)/10000);
        #    print("right ",right_range);
        #}
        #else if(left_range<right_range)
        #{
        #    setprop("/controls/flight/rudder",(right_range-left_range)*(right_range-left_range)/10000);
        #    print("left ",left_range);
        #}
        #else
        #    setprop("/controls/flight/rudder",0);
        props.getNode("/controls/flight/rudder",1).setValue((right_range-left_range)/200);
    }
};
var road_check_timer = maketimer(0.1,road_check_func);
var toggle_auto_pilot = func(){
    if(!road_check_timer.isRunning)
    {
        road_check_timer.start();
        props.getNode("/sim/messages/copilot",1).setValue("ze dong sheng teaan see tong yee tse yung. Auto Sheng Teaan System Activated!");
    }
    else
    {
        road_check_timer.stop();
        props.getNode("/sim/messages/copilot",1).setValue("ze dong sheng teaan see tong yee guan bee. Auto Sheng Teaan System is off.");
    }
}