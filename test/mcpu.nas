var inst={
    inst_stop:0,
    inst_mov_reg_reg:1,
    inst_mov_reg_imm_low:2,
    inst_mov_reg_imm_high:3,
    inst_add:4,
    inst_sub:5,
    inst_mult:6,
    inst_div:7,
    inst_and:8,
    inst_or:9,
    inst_xor:10,
    inst_not:11,
    inst_nand:12,
    inst_shl:13,
    inst_shr:14,
    inst_jmp:15,
    inst_jt:16,
    inst_jf:17,
    inst_les:18,
    inst_grt:19,
    inst_leq:20,
    inst_geq:21,
    inst_eq:22,
    inst_in:23,
    inst_out:24
};

var hex=func(){
    var vec=[];
    foreach(var i;['0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'])
        foreach(var j;['0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'])
            append(vec,i~j);
    return func(n){
        return vec[n];
    }
}();

var hex32=func(n){
    return hex(bits.u32_and(n/math.pow(2,24),0xff))~
    hex(bits.u32_and(n/math.pow(2,16),0xff))~
    hex(bits.u32_and(n/math.pow(2,8),0xff))~
    hex(bits.u32_and(n,0xff));
}

var machine=func(disk_file){

var reg=[];
var reg_size=32;
var pc=0;
var ir=[0,0,0,0]; # 32 bit instruction word
var mem=[];
var mem_size=1024*1024*4; # memory size, byte
var init=func(){
    println("[",os.time(),"] init ",reg_size," registers.");
    setsize(reg,reg_size); # 8 bit address wire
    for(var i=0;i<reg_size;i+=1)
        reg[i]=0;
    println("[",os.time(),"] init memory, memory size: ",mem_size/1024/1024," MB.");
    setsize(mem,mem_size);
    for(var i=0;i<mem_size;i+=1)
        mem[i]=0;
    println("[",os.time(),"] init completed.");
}
init();
var load=func(){
    var vec=split(" ",disk_file);
    println("[",os.time(),"] loading boot from disk: ",size(vec)," byte.");
    forindex(var i;vec)
        mem[i]=int("0x"~vec[i]);
    println("[",os.time(),"] loading complete.");
}
load();
var ctx_info=func(){
    var cnt=0;
    println("pc    : 0x",hex32(pc));
    println("instr : 0x",hex(ir[0]),hex(ir[1]),hex(ir[2]),hex(ir[3]));
    for(var i=0;i<reg_size;i+=1){
        print("reg[",hex(i),"]: 0x",hex32(reg[i])," ");
        if(cnt==3){
            print("\n");
            cnt=0;
        }else{
            cnt+=1;
        }
    }
}
var exec=func(info=1){
    println("[",os.time(),"] executing ...");
    while(1){
        ir=[mem[pc],mem[pc+1],mem[pc+2],mem[pc+3]];
        if(info)ctx_info();
        var op=ir[0];
        if(op==inst.inst_stop){
            break;
        }elsif(op==inst.inst_mov_reg_reg){
            reg[ir[1]]=reg[ir[2]];
        }elsif(op==inst.inst_mov_reg_imm_low){
            reg[ir[1]]=bits.u32_and(reg[ir[1]],0xffff0000);
            reg[ir[1]]=bits.u32_or(reg[ir[1]],ir[2]*math.pow(2,8)+ir[3]);
        }elsif(op==inst.inst_mov_reg_imm_high){
            reg[ir[1]]=bits.u32_and(reg[ir[1]],0x0000ffff);
            reg[ir[1]]=bits.u32_or(reg[ir[1]],ir[2]*math.pow(2,24)+ir[3]*math.pow(2,16));
        }elsif(op==inst.inst_add){
            reg[ir[1]]=bits.u32_and(reg[ir[2]]+reg[ir[3]],0xffffffff);
        }elsif(op==inst.inst_sub){
            reg[ir[1]]=bits.u32_and(reg[ir[2]]-reg[ir[3]],0xffffffff);
        }elsif(op==inst.inst_mult){
            reg[ir[1]]=bits.u32_and(reg[ir[2]]*reg[ir[3]],0xffffffff);
        }elsif(op==inst.inst_div){
            reg[ir[1]]=bits.u32_and(reg[ir[2]]/reg[ir[3]],0xffffffff);
        }elsif(op==inst.inst_and){
            reg[ir[1]]=bits.u32_and(reg[ir[2]],reg[ir[3]]);
        }elsif(op==inst.inst_or){
            reg[ir[1]]=bits.u32_or(reg[ir[2]],reg[ir[3]]);
        }elsif(op==inst.inst_xor){
            reg[ir[1]]=bits.u32_xor(reg[ir[2]],reg[ir[3]]);
        }elsif(op==inst.inst_not){
            reg[ir[1]]=bits.u32_not(reg[ir[2]]);
        }elsif(op==inst.inst_nand){
            reg[ir[1]]=bits.u32_nand(reg[ir[2]],reg[ir[3]]);
        }elsif(op==inst.inst_shl){
            reg[ir[1]]=bits.u32_and(reg[ir[2]]*math.pow(2,reg[ir[3]]),0xffffffff);
        }elsif(op==inst.inst_shr){
            reg[ir[1]]=bits.u32_and(reg[ir[2]]/math.pow(2,reg[ir[3]]),0xffffffff);
        }elsif(op==inst.inst_jmp){
            pc=reg[ir[1]];
        }elsif(op==inst.inst_jt){
            pc=reg[ir[1]]?reg[ir[2]]:pc;
        }elsif(op==inst.inst_jf){
            pc=reg[ir[1]]?pc:reg[ir[2]];
        }elsif(op==inst.inst_les){
            reg[ir[1]]=reg[ir[2]]<reg[ir[3]];
        }elsif(op==inst.inst_grt){
            reg[ir[1]]=reg[ir[2]]>reg[ir[3]];
        }elsif(op==inst.inst_leq){
            reg[ir[1]]=reg[ir[2]]<=reg[ir[3]];
        }elsif(op==inst.inst_geq){
            reg[ir[1]]=reg[ir[2]]>=reg[ir[3]];
        }elsif(op==inst.inst_eq){
            reg[ir[1]]=reg[ir[2]]==reg[ir[3]];
        }elsif(op==inst.inst_in){
            reg[0]=0; # unfinished
        }elsif(op==inst.inst_out){
            println("reg[",ir[1],"]: 0x",hex32(reg[ir[1]]));
        }
        pc+=4;
    }
    println("[",os.time(),"] execute complete.");
};
    return {exec:exec};
}(
hex(inst.inst_mov_reg_imm_high)~" 01 ca fe "~ # reg[1]=0xcafe0000
hex(inst.inst_mov_reg_imm_low)~" 01 ba be "~  # reg[1]=0xcafebabe
hex(inst.inst_out)~" 01 00 00 "~              # output reg[1]
hex(inst.inst_mov_reg_imm_low)~" 02 00 20 "~  # reg[2]=0x10
hex(inst.inst_jmp)~" 02 00 00 "~              # jmp *reg[2]
hex(inst.inst_out)~" 01 00 00 "~
hex(inst.inst_out)~" 01 00 00 "~
hex(inst.inst_out)~" 01 00 00 "~
hex(inst.inst_out)~" 01 00 00 "~
hex(inst.inst_out)~" 01 00 00 "~              # should jump here
hex(inst.inst_jf)~" 01 02 00 "~               # should not jump
hex(inst.inst_mov_reg_imm_low)~" 03 00 04 "~  # reg[3]=4
hex(inst.inst_mov_reg_imm_low)~" 04 00 00 "~  # reg[4]=0
hex(inst.inst_out)~" 04 00 00 "~              # output reg[4]
hex(inst.inst_grt)~" 00 04 03 "~              # reg[0]=reg[4]>reg[3]
hex(inst.inst_mov_reg_imm_low)~" 05 00 30 "~  # reg[5]=0x2c
hex(inst.inst_mov_reg_imm_low)~" 06 00 01 "~  # reg[6]=1
hex(inst.inst_add)~" 04 04 06 "~              # reg[4]+=reg[6]
hex(inst.inst_jf)~" 00 05 00 "                # jmp *reg[5] if reg[0]!=true
);

machine.exec(0);