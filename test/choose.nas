if(this_token.type==__if)
{
    parse.push(this_token);
    return;
}
elsif(this_token.type==__elsif)
{
    parse.push(this_token);
    return;
}
else if(this_token.type!=__else)
{
    exit(0);
}
elsif(this_token.type==__elsif)
{
    parse.push(this_token);
    return;
}
elsif(this_token.type==__elsif)
{
    parse.push(this_token);
    return;
}
else if(this==(1+2+3*1))
{
    return nil;
}
else
{
    parse.push(this_token);
    return;
}

if(!id)
{
    exit(0);
}
