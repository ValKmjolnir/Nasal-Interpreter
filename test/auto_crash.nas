# Road check and auto pilot by ValKmjolnir
import.stl.fg_env;

var dt=0.01;
var intergral=0;
var derivative=0;
var previous_error=0;
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

    if((position_names[0]=="Freeway") or (position_names[0]=="Road")){
        var car_heading = 0;
        var lat_change  = 0;
        var lon_change  = 0;
        var left_range  = 0;
        var right_range = 0;

        for(var i=0;i>-0.00005;i-=0.000001){
            car_heading = props.getNode("/orientation/heading-deg",1).getValue();
            lat_change  = math.sin(D2R*car_heading);
            lon_change  = -math.cos(D2R*car_heading);
            lat = props.getNode("/position/latitude-deg",1).getValue()+0.0001*math.cos(D2R*car_heading);
            lon = props.getNode("/position/longitude-deg",1).getValue()+0.0001*math.sin(D2R*car_heading);
            var other_position_info = geodinfo(position_change(lat,i*lat_change),position_change(lon,i*lon_change));
            var other_names = other_position_info[1].names;
            if((other_names[0]=="Freeway") or (other_names[0]=="Road"))
                right_range += 1;
            else
                break;
        }
        for(var i=0;i<0.00005;i+=0.000001){
            car_heading = props.getNode("/orientation/heading-deg",1).getValue();
            lat_change  = math.sin(D2R*car_heading);
            lon_change  = -math.cos(D2R*car_heading);
            lat = props.getNode("/position/latitude-deg",1).getValue()+0.0001*math.cos(D2R*car_heading);
            lon = props.getNode("/position/longitude-deg",1).getValue()+0.0001*math.sin(D2R*car_heading);
            var other_position_info = geodinfo(position_change(lat,i*lat_change),position_change(lon,i*lon_change));
            var other_names = other_position_info[1].names;
            if((other_names[0]=="Freeway") or (other_names[0]=="Road"))
                left_range+=1;
            else
                break;
        }
        var error=right_range-left_range;
        intergral+=error*dt;
        derivative=(error-previous_error)/dt;
        var (Kp,Ki,Kd)=(1/900,0.05,0.005);
        # print("change p ",Kp*error*error,' i ',Ki*intergral,' d ',Kd*derivative);
        if(error<0)
            props.getNode("/", 1).setValue("/controls/flight/rudder",-Kp*error*error+Ki*intergral+Kd*derivative);
        else if(error>0)
            props.getNode("/", 1).setValue("/controls/flight/rudder",Kp*error*error+Ki*intergral+Kd*derivative);
        else
            props.getNode("/", 1).setValue("/controls/flight/rudder",0);
        
        # for simulation test, in fg these three lines are deleted
        println(" rudder :",props.getNode("/controls/flight/rudder",1).getValue());
        println(" dt :",dt,'\tintergral :',intergral,'\tderivative :',derivative);
        println(" prev-err :",previous_error,'\terror :',error);

        previous_error=error;
    }
};

var road_check_timer = maketimer(0.01,road_check_func);
var toggle_auto_pilot = func(){
    if(!road_check_timer.isRunning){
        intergral=0;
        road_check_timer.start();
        props.getNode("/sim/messages/copilot",1).setValue('/',"ze dong sheng teaan see tong yee tse yung. Auto Sheng Teaan System Activated!");
    }else{
        road_check_timer.stop();
        props.getNode("/sim/messages/copilot",1).setValue('/',"ze dong sheng teaan see tong yee guan bee. Auto Sheng Teaan System is off.");
    }
}

# this is used to simulate the running process in fg
# when using in fg, delete these lines below
toggle_auto_pilot();
road_check_timer.restart(0.1);
simulation();