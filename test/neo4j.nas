import("lib.nas");

rand(time(0));
var chartable=split('','abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789');
var node=func(type)
{
    var s="";
    for(var i=0;i<10;i+=1)
        s~=chartable[rand()*62];
    return {name:s,type:type,next:[]};
}
var film_node=[];
for(var i=0;i<1000;i+=1)
    append(film_node,node("film"));
var director_node=[];
for(var i=0;i<400;i+=1)
    append(director_node,node("direct"));
var actor_node=[];
for(var i=0;i<2000;i+=1)
    append(actor_node,node("actor"));
var writer_node=[];
for(var i=0;i<300;i+=1)
    append(writer_node,node("writer"));
var type_node=[];
for(var i=0;i<20;i+=1)
    append(type_node,node("type"));
var lang_node=[];
for(var i=0;i<120;i+=1)
    append(lang_node,node("lang"));
var country_node=[];
for(var i=0;i<120;i+=1)
    append(country_node,node("country"));

func()
{
    var director_size=size(director_node);
    var actor_size=size(actor_node);
    var writer_size=size(writer_node);
    var type_size=size(type_node);
    var lang_size=size(lang_node);
    var country_size=size(country_node);

    var director_link=int(1+rand()*2);
    var actor_link=int(1+rand()*10);
    var writer_link=int(1+rand());
    var type_link=int(1+rand()*3);
    var lang_link=int(1+rand()*4);
    var country_link=int(1+rand()*2);
    foreach(var film;film_node)
    {
        for(var i=0;i<director_link;i+=1)
        {
            var director=director_node[rand()*director_size];
            append(film.next,director);
            append(director.next,film);
        }
        for(var i=0;i<actor_link;i+=1)
        {
            var actor=actor_node[rand()*actor_size];
            append(film.next,actor);
            append(actor.next,film);
        }
        for(var i=0;i<writer_link;i+=1)
        {
            var writer=writer_node[rand()*writer_size];
            append(film.next,writer);
            append(writer.next,film);
        }
        for(var i=0;i<type_link;i+=1)
        {
            var _type=type_node[rand()*type_size];
            append(film.next,_type);
            append(_type.next,film);
        }
        for(var i=0;i<lang_link;i+=1)
        {
            var lang=lang_node[rand()*lang_size];
            append(film.next,lang);
            append(lang.next,film);
        }
        for(var i=0;i<country_link;i+=1)
        {
            var country=country_node[rand()*country_size];
            append(film.next,country);
            append(country.next,film);
        }
    }
    return;
}();


var film_list=[];
var count_list=[];
for(var i=0;i<10;i+=1)
{
    append(film_list,film_node[i]);
    append(count_list,1);
}

var sort_list=func(begin,end)
{
    for(var i=begin;i<end;i+=1)
    {
        var index=i;
        for(var j=i+1;j<end;j+=1)
            if(count_list[index]<count_list[j])
                index=j;
        if(index!=i)
        {
            var tmp=film_list[i];
            film_list[i]=film_list[index];
            film_list[index]=tmp;
            tmp=count_list[i];
            count_list[i]=count_list[index];
            count_list[index]=tmp;
        }
    }
    return;
}

while(1)
{
    var list_size=size(film_list);
    list_size=list_size>10?10:list_size;
    for(var i=0;i<list_size;i+=1)
        print(i,'\t:',film_list[i].name,'\t',count_list[i]);
    var choose=input();
    if(choose=="exit")
        break;
    if(num(choose)>=list_size)
        die("choose a correct index");
    var label_list=film_node[num(choose)].next;
    film_list=[];
    count_list=[];
    foreach(var label;label_list)
        foreach(var film;label.next)
        {
            var has=0;
            for(var i=0;i<size(film_list);i+=1)
                if(film_list[i].name==film.name)
                {
                    has=1;
                    count_list[i]+=rand();
                    break;
                }
            if(has==0)
            {
                append(film_list,film);
                append(count_list,1);
            }
        }
    sort_list(0,size(film_list));
}

foreach(var film;film_node)
    setsize(film.next,0);
