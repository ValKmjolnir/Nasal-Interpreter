# game left in corner by ValKmjolnir
# 2020

# lib function defined here
var print=func(elements...)
{
    nasal_call_builtin_std_cout(elements);
    return nil;
};
var append=func(vector,elements...)
{
    nasal_call_builtin_push_back(vector,elements);
    return nil;
}
var setsize=func(vector,size)
{
    nasal_call_builtin_set_size(vector,size);
    return nil;
}
var system=func(str)
{
    nasal_call_builtin_system(str);
    return;
}
var sleep=func(duration)
{
    nasal_call_builtin_sleep(duration);
    return;
}
var input=func()
{
    return nasal_call_builtin_input();
}
var io=
{
    fin:func(filename)
    {
        return nasal_call_builtin_finput(filename);
    },
    fout:func(filename,str)
    {
        nasal_call_builtin_foutput(filename,str);
        return;
    }
};
var int=func(str)
{
    return str+0;
}
var str=func(num)
{
    return num~'';
}
# string split

# game elements defined here
var role_property=
{
    health:100,
    mood:100,
    satiety:100,
    thirst:100,
    health_change:func(x)
    {
        me.health+=x;
        if(me.health<0)
            me.health=0;
        elsif(me.health>100)
            me.health=100;
        return nil;
    },
    mood_change:func(x)
    {
        me.mood+=x;
        if(me.mood<0)
            me.mood=0;
        elsif(me.mood>100)
            me.mood=100;
        return nil;
    },
    satiety_change:func(x)
    {
        me.satiety+=x;
        if(me.satiety<0)
            me.satiety=0;
        elsif(me.satiety>100)
            me.satiety=100;
        return nil;
    },
    thirst_change:func(x)
    {
        me.thirst+=x;
        if(me.thirst<0)
            me.thirst=0;
        elsif(me.thirst>100)
            me.thirst=100;
        return nil;
    }
};
var screen=
{
    picture:[],
    info_below_left:[],
    info_below_right:[],
    clear:func()
    {
        me.picture=[];
        me.info_below_left=[];
        me.info_below_right=[];
        return;
    },
    pic_addline:func(_str)
    {
        append(me.picture,_str);
        return;
    },
    left_add:func(_str)
    {
        append(me.info_below_left,_str);
        return;
    },
    right_add:func(_str)
    {
        append(me.info_below_right,_str);
        return;
    },
    prt_screen:func()
    {
        foreach(var i;me.picture)
            print(i);
        forindex(var i;me.info_below_left)
            print(me.info_below_left[i]~me.info_below_right[i]);
        return;
    }
};
var first_shown_info=func()
{
    system("cls");
    var str_list=[
        "+-----------------------------------------------+",
        "|                                               |",
        "|                                               |",
        "| Let me tell you a story...                    |",
        "| A story that really happened many years ago...|",
        "| Nearly no one knows and cares about it...     |",
        "| But some children may still suffer from...    |",
        "| This kind of stories...                       |",
        "| And this kind of stories never stop hurting...|",
        "| People that are still alive...                |",
        "|                                               |",
        "|                                               |",
        "+-----------------------------------------------+"
    ];
    foreach(var i;str_list)
        print(i);
    return;
}
var generate_role_property=func()
{
    screen.left_add("+-----------------------+");
    var str="";
    for(var i=10;i<=100;i+=10)
    {
        if(i<=role_property.health)
            str~="=";
        else
            str~=" ";
    }
    screen.left_add("|[health ]:"~str~"   |");
    str="";
    for(var i=10;i<=100;i+=10)
    {
        if(i<=role_property.mood)
            str~="=";
        else
            str~=" ";
    }
    screen.left_add("|[mood   ]:"~str~"   |");
    str="";
    for(var i=10;i<=100;i+=10)
    {
        if(i<=role_property.satiety)
            str~="=";
        else
            str~=" ";
    }
    screen.left_add("|[satiety]:"~str~"   |");
    str="";
    for(var i=10;i<=100;i+=10)
    {
        if(i<=role_property.thirst)
            str~="=";
        else
            str~=" ";
    }
    screen.left_add("|[thirst ]:"~str~"   |");
    screen.left_add("+-----------------------+");
    return;
}
var generate_choose_list=func()
{
    var str_list=[
        "-----------------------+",
        "[1]| next step         |",
        "[2]| restart           |",
        "[3]| store game        |",
        "[4]| exit              |",
        "-----------------------+"
    ];
    foreach(var i;str_list)
        screen.right_add(i);
    return;
}
var next_step=func()
{
    role_property.health_change(-1);
    role_property.mood_change(-1);
    role_property.satiety_change(-1);
    role_property.thirst_change(-10);
    var str_list=[
        "+-----------------------------------------------+",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "+-----------------------------------------------+"
    ];
    foreach(var i;str_list)
        screen.pic_addline(i);
    return;
}
var restart=func()
{
    role_property.health=100;
    role_property.mood=100;
    role_property.satiety=100;
    role_property.thirst=100;
    var str_list=[
        "+-----------------------------------------------+",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "+-----------------------------------------------+"
    ];
    foreach(var i;str_list)
        screen.pic_addline(i);
    return;
}
var generate_incorrect_choice_screen=func()
{
    var str_list=[
        "+-----------------------------------------------+",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|            make a correct choice.             |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "+-----------------------------------------------+"
    ];
    foreach(var i;str_list)
        screen.pic_addline(i);
}
var generate_goodbye_screen=func()
{
    var str_list=[
        "+-----------------------------------------------+",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|              see you next life.               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "+-----------------------------------------------+"
    ];
    foreach(var i;str_list)
        screen.pic_addline(i);
}
var store_file=func()
{
    var str=role_property.health~'\n'~role_property.mood~'\n'~role_property.satiety~'\n'~role_property.thirst~'\n';
    io.fout("game-left-in-corner.glic",str);
    var str_list=[
        "+-----------------------------------------------+",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                  data stored.                 |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "|                                               |",
        "+-----------------------------------------------+"
    ];
    foreach(var i;str_list)
        screen.pic_addline(i);
    return;
}
var get_file=func()
{
    var str=io.fin("game-left-in-corner.glic");
    print(str);
    return;
}
var game_main=func()
{
    first_shown_info();
    screen.clear();
    generate_role_property();
    generate_choose_list();
    screen.prt_screen();
    while(1)
    {
        screen.clear();
        print("|your choice[1|2|3|4]:                          |");
        var choice=input();
        if((choice!='1') and (choice!='2') and (choice!='3') and (choice!='4'))
            generate_incorrect_choice_screen();
        elsif(choice=='1')
            next_step();
        elsif(choice=='2')
            restart();
        elsif(choice=='3')
            store_file();
        elsif(choice=='4')
        {
            system("cls");
            screen.clear();
            generate_goodbye_screen();
            generate_role_property();
            generate_choose_list();
            screen.prt_screen();
            break;
        }
        system("cls");
        generate_role_property();
        generate_choose_list();
        screen.prt_screen();
        if(role_property.health==0 or role_property.mood==0 or role_property.satiety==0 or role_property.thirst==0)
        {
            print("|you died.                                      |");
            print("+-----------------------------------------------+");
            system("pause");
            screen.clear();
            restart();
            system("cls");
            generate_role_property();
            generate_choose_list();
            screen.prt_screen();
        }
    }
    return;
}
game_main();