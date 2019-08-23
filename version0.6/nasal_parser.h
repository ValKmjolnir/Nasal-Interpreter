#ifndef __NASAL_PARSER_H__
#define __NASAL_PARSER_H__

#include <cstring>
#include <stack>
#include <list>

enum token_type
{
	__stack_end,
	__equal,// =
	__cmp_equal,// ==
	__cmp_not_equal,// !=
	__cmp_less,__cmp_less_or_equal,// < <=
	__cmp_more,__cmp_more_or_equal,// > >=
	__and_operator,__or_operator,__nor_operator,// and or !
	__add_operator,__sub_operator,__mul_operator,__div_operator,__link_operator,// + - * / ~
	__add_equal,__sub_equal,__mul_equal,__div_equal,__link_equal,// += -= *= /= ~=
	__left_brace,__right_brace,// {}
	__left_bracket,__right_bracket,// []
	__left_curve,__right_curve,// ()
	__semi,// ;
	__comma,// ,
	__colon,// :
	__dot,// .
	__var,// var reserve word
	__func,// func reserve word
	__unknown_type_id,__identifier,__identifiers,
	__scalar,__scalars,
	__hash_member,__hash_members,
	__statement,__statements,
	__function,//function(){}
	__definition,__assignment,
	__loop,__continue,__break,__for,__forindex,__foreach,__while,// for()while() continue; break;
	__choose,__if,__elsif,__else,// if else if else
	__return
};

void print_token_type(int type)
{
	std::string context="";
	switch(type)
	{
		case __stack_end:
			context="__stack_end";
			break;
		case __equal:
			context="=";
			break;
		case __cmp_equal:
			context="==";
			break;
		case __cmp_not_equal:
			context="!=";
			break;
		case __cmp_less:
			context="<";
			break;
		case __cmp_less_or_equal:
			context="<=";
			break;
		case __cmp_more:
			context=">";
			break;
		case __cmp_more_or_equal:
			context=">=";
			break;
		case __and_operator:
			context="and";
			break;
		case __or_operator:
			context="or";
			break;
		case __nor_operator:
			context="!";
			break;
		case __add_operator:
			context="+";
			break;
		case __sub_operator:
			context="-";
			break;
		case __mul_operator:
			context="*";
			break;
		case __div_operator:
			context="/";
			break;
		case __link_operator:
			context="~";
			break;
		case __add_equal:
			context="+=";
			break;
		case __sub_equal:
			context="-=";
			break;
		case __mul_equal:
			context="*=";
			break;
		case __div_equal:
			context="/=";
			break;
		case __link_equal:
			context="~=";
			break;
		case __left_brace:
			context="{";
			break;
		case __right_brace:
			context="}";
			break;
		case __left_bracket:
			context="[";
			break;
		case __right_bracket:
			context="]";
			break;
		case __left_curve:
			context="(";
			break;
		case __right_curve:
			context=")";
			break;
		case __semi:
			context=";";
			break;
		case __comma:
			context=",";
			break;
		case __colon:
			context=":";
			break;
		case __dot:
			context=".";
			break;
		case __var:
			context="var";
			break;
		case __func:
			context="func";
			break;
		case __unknown_type_id:
			context="__uid";
			break;
		case __identifier:
			context="__id";
			break;
		case __identifiers:
			context="__ids";
			break;
		case __scalar:
			context="__scl";
			break;
		case __scalars:
			context="__scls";
			break;
		case __hash_member:
			context="__hmem";
			break;
		case __hash_members:
			context="__hmems";
			break;
		case __continue:
			context="continue";
			break;
		case __break:
			context="break";
			break;
		case __for:
			context="for";
			break;
		case __forindex:
			context="forindex";
			break;
		case __foreach:
			context="foreach";
			break;
		case __while:
			context="while";
			break;
		case __if:
			context="if";
			break;
		case __elsif:
			context="elsif";
			break;
		case __else:
			context="else";
			break;
		case __return:
			context="return";
			break;
	}
	std::cout<<context;
	return;
}

struct parse_unit
{
	int line;
	int type;
	std::string content;
};

class nasal_parser
{
	private:
		std::stack<parse_unit> parser;
	public:
		void stack_set_empty()
		{
			while(!parser.empty())
				parser.pop();
			return;
		}
		bool scalar_check()
		{
			int tbl[4]={0};
			std::stack<parse_unit> temp;
			for(int i=0;i<4;++i)
			{
				if(parser.empty())
					break;
				temp.push(parser.top());
				tbl[i]=temp.top().type;
				parser.pop();
			}
			for(int i=0;i<4;++i)
			{
				if(temp.empty())
					break;
				parser.push(temp.top());
				temp.pop();
			}
			if((tbl[1]==__left_bracket) && (tbl[0]==__right_bracket))
			{
				parse_unit t;
				t.type=__scalar;// !
				t.line=parser.top().line;
				for(int i=0;i<2;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[1]==__left_brace) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__scalar;// !
				t.line=parser.top().line;
				for(int i=0;i<2;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[3]!=__unknown_type_id) && (tbl[2]==__left_bracket) && ((tbl[1]==__identifier) || (tbl[1]==__identifiers) || (tbl[1]==__scalar) || (tbl[1]==__scalars)) && (tbl[0]==__right_bracket))
			{
				parse_unit t;
				t.type=__scalar;// !
				t.line=parser.top().line;
				for(int i=0;i<3;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[3]!=__unknown_type_id) && (tbl[2]==__left_brace) && ((tbl[1]==__hash_member) || (tbl[1]==__hash_members)) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__scalar;// !
				t.line=parser.top().line;
				for(int i=0;i<3;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[2]!=__scalar) && (tbl[2]!=__identifier) && (tbl[1]==__sub_operator) && (tbl[0]==__scalar))
			{
				parse_unit t;
				t.type=__scalar;// !
				t.line=parser.top().line;
				for(int i=0;i<2;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			return false;
		}
		bool scalars_reduction()
		{
			int tbl[3]={0};
			std::stack<parse_unit> temp;
			for(int i=0;i<3;++i)
			{
				if(parser.empty())
					break;
				temp.push(parser.top());
				tbl[i]=temp.top().type;
				parser.pop();
			}
			for(int i=0;i<3;++i)
			{
				if(temp.empty())
					break;
				parser.push(temp.top());
				temp.pop();
			}
			if(((tbl[2]==__scalar) || (tbl[2]==__scalars)) && (tbl[1]==__comma) && ((tbl[0]==__scalar) || (tbl[0]==__identifier)))
			{
				parse_unit t;
				t.type=__scalars;
				t.line=parser.top().line;
				for(int i=0;i<3;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if(((tbl[2]==__identifier) || (tbl[2]==__identifiers)) && (tbl[1]==__comma) && (tbl[0]==__scalar))
			{
				parse_unit t;
				t.type=__scalars;
				t.line=parser.top().line;
				for(int i=0;i<3;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			return false;
		}
		bool identifier_check()
		{
			int tbl[4]={0};
			std::stack<parse_unit> temp;
			for(int i=0;i<4;++i)
			{
				if(parser.empty())
					break;
				temp.push(parser.top());
				tbl[i]=temp.top().type;
				parser.pop();
			}
			for(int i=0;i<4;++i)
			{
				if(temp.empty())
					break;
				parser.push(temp.top());
				temp.pop();
			}
			if((tbl[1]==__unknown_type_id) && ((tbl[0]!=__dot) && (tbl[0]!=__left_curve) && (tbl[0]!=__left_bracket)))
			{
				parse_unit t;
				parse_unit temp_top=parser.top();
				t.type=__identifier;
				t.line=parser.top().line;
				for(int i=0;i<2;++i)
					parser.pop();
				parser.push(t);
				parser.push(temp_top);
				return true;
			}
			else if((tbl[3]==__unknown_type_id) && (tbl[2]==__left_bracket) && ((tbl[1]==__identifier) || (tbl[1]==__scalar)) && (tbl[0]==__right_bracket))
			{
				parse_unit t;
				t.type=__identifier;
				t.line=parser.top().line;
				for(int i=0;i<4;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[2]!=__scalar) && (tbl[2]!=__identifier) && (tbl[1]==__sub_operator) && (tbl[0]==__identifier))
			{
				parse_unit t;
				t.type=__identifier;
				t.line=parser.top().line;
				for(int i=0;i<2;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[2]==__unknown_type_id) && (tbl[1]==__dot) && (tbl[0]==__identifier))
			{
				parse_unit t;
				t.type=__identifier;
				t.line=parser.top().line;
				for(int i=0;i<3;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[3]==__identifier) && (tbl[2]==__left_curve) && ((tbl[1]==__identifier) || (tbl[1]==__identifiers) || (tbl[1]==__scalar) || (tbl[1]==__scalars)) && (tbl[0]==__right_curve))
			{
				parse_unit t;
				t.type=__identifier;
				t.line=parser.top().line;
				for(int i=0;i<4;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[2]==__identifier) && (tbl[1]==__left_curve) && (tbl[0]==__right_curve))
			{
				parse_unit t;
				t.type=__identifier;
				t.line=parser.top().line;
				for(int i=0;i<3;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			return false;
		}
		bool identifiers_reduction()
		{
			int tbl[3]={0};
			std::stack<parse_unit> temp;
			for(int i=0;i<3;++i)
			{
				if(parser.empty())
					break;
				temp.push(parser.top());
				tbl[i]=temp.top().type;
				parser.pop();
			}
			for(int i=0;i<3;++i)
			{
				if(temp.empty())
					break;
				parser.push(temp.top());
				temp.pop();
			}
			if(((tbl[2]==__identifier) || (tbl[2]==__identifiers)) && (tbl[1]==__comma) && (tbl[0]==__identifier))
			{
				parse_unit t;
				t.type=__identifiers;
				t.line=parser.top().line;
				for(int i=0;i<3;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			return false;
		}
		bool hashmember_check()
		{
			int tbl[9]={0};
			std::stack<parse_unit> temp;
			for(int i=0;i<9;++i)
			{
				if(parser.empty())
					break;
				temp.push(parser.top());
				tbl[i]=temp.top().type;
				parser.pop();
			}
			for(int i=0;i<9;++i)
			{
				if(temp.empty())
					break;
				parser.push(temp.top());
				temp.pop();
			}
			if((tbl[2]==__identifier) && (tbl[1]==__colon) && ((tbl[0]==__identifier) || (tbl[0]==__scalar)))
			{
				parse_unit t;
				t.type=__hash_member;
				t.line=parser.top().line;
				for(int i=0;i<3;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[6]==__identifier) && (tbl[5]==__colon) && (tbl[4]==__func) && (tbl[3]==__left_curve) && (tbl[2]==__right_curve) && (tbl[1]==__left_brace) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__hash_member;
				t.line=parser.top().line;
				for(int i=0;i<7;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[7]==__identifier) && (tbl[6]==__colon) && (tbl[5]==__func) && (tbl[4]==__left_curve) && ((tbl[3]==__identifier) || (tbl[3]==__identifiers)) && (tbl[2]==__right_curve) && (tbl[1]==__left_brace) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__hash_member;
				t.line=parser.top().line;
				for(int i=0;i<8;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[7]==__identifier) && (tbl[6]==__colon) && (tbl[5]==__func) && (tbl[4]==__left_curve) && (tbl[3]==__right_curve) && (tbl[2]==__left_brace) && ((tbl[1]==__statement) || (tbl[1]==__statements)) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__hash_member;
				t.line=parser.top().line;
				for(int i=0;i<8;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[8]==__identifier) && (tbl[7]==__colon) && (tbl[6]==__func) && (tbl[5]==__left_curve) && ((tbl[4]==__identifier) || (tbl[4]==__identifiers)) && (tbl[3]==__right_curve) && (tbl[2]==__left_brace) && ((tbl[1]==__statement) || (tbl[1]==__statements)) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__hash_member;
				t.line=parser.top().line;
				for(int i=0;i<9;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			return false;
		}
		bool hashmembers_reduction()
		{
			int tbl[3]={0};
			std::stack<parse_unit> temp;
			for(int i=0;i<3;++i)
			{
				if(parser.empty())
					break;
				temp.push(parser.top());
				tbl[i]=temp.top().type;
				parser.pop();
			}
			for(int i=0;i<3;++i)
			{
				if(temp.empty())
					break;
				parser.push(temp.top());
				temp.pop();
			}
			if(((tbl[2]==__hash_member) || (tbl[2]==__hash_members)) && (tbl[1]==__comma) && (tbl[0]==__hash_member))
			{
				parse_unit t;
				t.type=__hash_members;
				t.line=parser.top().line;
				for(int i=0;i<3;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			return false;
		}
		bool calculation_reduction() //use __scalar
		{
			int tbl[3]={0};
			std::stack<parse_unit> temp;
			for(int i=0;i<3;++i)
			{
				if(parser.empty())
					break;
				temp.push(parser.top());
				tbl[i]=temp.top().type;
				parser.pop();
			}
			for(int i=0;i<3;++i)
			{
				if(temp.empty())
					break;
				parser.push(temp.top());
				temp.pop();
			}
			if((tbl[2]==__left_curve) && ((tbl[1]==__identifier) || (tbl[1]==__scalar)) && (tbl[0]==__right_curve))
			{
				parse_unit t;
				t.type=__scalar;
				t.line=parser.top().line;
				for(int i=0;i<3;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			if((tbl[1]==__nor_operator) && ((tbl[0]==__identifier) || (tbl[0]==__scalar)))
			{
				parse_unit t;
				t.type=__scalar;
				t.line=parser.top().line;
				for(int i=0;i<2;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if(((tbl[2]==__identifier) || (tbl[2]==__scalar))
					&& ((tbl[1]==__add_operator) || (tbl[1]==__sub_operator)
						|| (tbl[1]==__mul_operator) || (tbl[1]==__div_operator)
						|| (tbl[1]==__link_operator) || (tbl[1]==__and_operator)
						|| (tbl[1]==__or_operator) || (tbl[1]==__cmp_equal) || (tbl[1]==__cmp_not_equal)
						|| (tbl[1]==__cmp_less) || (tbl[1]==__cmp_less_or_equal)
						|| (tbl[1]==__cmp_more) || (tbl[1]==__cmp_more_or_equal))
					&& ((tbl[0]==__identifier) || (tbl[0]==__scalar)))
			{
				parse_unit t;
				t.type=__scalar;
				t.line=parser.top().line;
				for(int i=0;i<3;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			return false;
		}
		bool definition_check()
		{
			int tbl[7]={0};
			std::stack<parse_unit> temp;
			for(int i=0;i<7;++i)
			{
				if(parser.empty())
					break;
				temp.push(parser.top());
				tbl[i]=temp.top().type;
				parser.pop();
			}
			for(int i=0;i<7;++i)
			{
				if(temp.empty())
					break;
				parser.push(temp.top());
				temp.pop();
			}
			if((tbl[4]==__var) && (tbl[3]==__identifier) && (tbl[2]==__equal) && ((tbl[1]==__identifier) || (tbl[1]==__scalar)) && (tbl[0]==__semi))
			{
				parse_unit t;
				t.type=__definition;
				t.line=parser.top().line;
				for(int i=0;i<5;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[5]==__var) && (tbl[4]==__identifier) && (tbl[3]==__equal) && (tbl[2]==__left_bracket) && (tbl[1]==__right_bracket) && (tbl[0]==__semi))
			{
				parse_unit t;
				t.type=__definition;
				t.line=parser.top().line;
				for(int i=0;i<6;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[5]==__var) && (tbl[4]==__identifier) && (tbl[3]==__equal) && (tbl[2]==__left_brace) && (tbl[1]==__right_brace) && (tbl[0]==__semi))
			{
				parse_unit t;
				t.type=__definition;
				t.line=parser.top().line;
				for(int i=0;i<6;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[6]==__var) && (tbl[5]==__identifier) && (tbl[4]==__equal) && (tbl[3]==__left_bracket) && ((tbl[2]==__scalar) || (tbl[2]==__scalars) || (tbl[2]==__identifier) || (tbl[2]==__identifiers)) && (tbl[1]==__right_bracket) && (tbl[0]==__semi))
			{
				parse_unit t;
				t.type=__definition;
				t.line=parser.top().line;
				for(int i=0;i<7;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[6]==__var) && (tbl[5]==__identifier) && (tbl[4]==__equal) && (tbl[3]==__left_brace) && ((tbl[2]==__hash_member) || (tbl[2]==__hash_members)) && (tbl[1]==__right_brace) && (tbl[0]==__semi))
			{
				parse_unit t;
				t.type=__definition;
				t.line=parser.top().line;
				for(int i=0;i<7;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			return false;
		}
		bool assignment_check()
		{
			int tbl[6]={0};
			std::stack<parse_unit> temp;
			for(int i=0;i<6;++i)
			{
				if(parser.empty())
					break;
				temp.push(parser.top());
				tbl[i]=temp.top().type;
				parser.pop();
			}
			for(int i=0;i<6;++i)
			{
				if(temp.empty())
					break;
				parser.push(temp.top());
				temp.pop();
			}
			if((tbl[3]==__identifier)
				&& ((tbl[2]==__equal) || (tbl[2]==__add_equal) || (tbl[2]==__sub_equal)
					|| (tbl[2]==__mul_equal) || (tbl[2]==__div_equal) || (tbl[2]==__link_equal))
				&& ((tbl[1]==__identifier) || (tbl[1]==__scalar)) && (tbl[0]==__semi))
			{
				parse_unit t;
				t.type=__assignment;
				t.line=parser.top().line;
				for(int i=0;i<4;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[4]==__identifier) && (tbl[3]==__equal) && (tbl[2]==__left_bracket) && (tbl[1]==__right_bracket) && (tbl[0]==__semi))
			{
				parse_unit t;
				t.type=__assignment;
				t.line=parser.top().line;
				for(int i=0;i<5;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[4]==__identifier) && (tbl[3]==__equal) && (tbl[2]==__left_brace) && (tbl[1]==__right_brace) && (tbl[0]==__semi))
			{
				parse_unit t;
				t.type=__assignment;
				t.line=parser.top().line;
				for(int i=0;i<5;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[5]==__identifier) && (tbl[4]==__equal) && (tbl[3]==__left_bracket) && ((tbl[2]==__scalar) || (tbl[2]==__scalars) || (tbl[2]==__identifier) || (tbl[2]==__identifiers)) && (tbl[1]==__right_bracket) && (tbl[0]==__semi))
			{
				parse_unit t;
				t.type=__assignment;
				t.line=parser.top().line;
				for(int i=0;i<6;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[5]==__identifier) && (tbl[4]==__equal) && (tbl[3]==__left_brace) && ((tbl[2]==__hash_member) || (tbl[2]==__hash_members)) && (tbl[1]==__right_brace) && (tbl[0]==__semi))
			{
				parse_unit t;
				t.type=__assignment;
				t.line=parser.top().line;
				for(int i=0;i<6;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			return false;
		}
		bool function_def()
		{
			int tbl[10]={0};
			std::stack<parse_unit> temp;
			for(int i=0;i<10;++i)
			{
				if(parser.empty())
					break;
				temp.push(parser.top());
				tbl[i]=temp.top().type;
				parser.pop();
			}
			for(int i=0;i<10;++i)
			{
				if(temp.empty())
					break;
				parser.push(temp.top());
				temp.pop();
			}
			if((tbl[7]==__var) && (tbl[6]==__identifier) && (tbl[5]==__equal) && (tbl[4]==__func) && (tbl[3]==__left_curve) && (tbl[2]==__right_curve) && (tbl[1]==__left_brace) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__function;
				t.line=parser.top().line;
				for(int i=0;i<8;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[8]==__var) && (tbl[7]==__identifier) && (tbl[6]==__equal) && (tbl[5]==__func) && (tbl[4]==__left_curve) && ((tbl[3]==__identifier) || (tbl[3]==__identifiers)) && (tbl[2]==__right_curve) && (tbl[1]==__left_brace) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__function;
				t.line=parser.top().line;
				for(int i=0;i<9;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[8]==__var) && (tbl[7]==__identifier) && (tbl[6]==__equal) && (tbl[5]==__func) && (tbl[4]==__left_curve) && (tbl[3]==__right_curve) && (tbl[2]==__left_brace) && ((tbl[1]==__statement) || (tbl[1]==__statements)) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__function;
				t.line=parser.top().line;
				for(int i=0;i<9;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[9]==__var) && (tbl[8]==__identifier) && (tbl[7]==__equal) && (tbl[6]==__func) && (tbl[5]==__left_curve) && ((tbl[4]==__identifier) || (tbl[4]==__identifiers)) && (tbl[3]==__right_curve) && (tbl[2]==__left_brace) && ((tbl[1]==__statement) || (tbl[1]==__statements)) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__function;
				t.line=parser.top().line;
				for(int i=0;i<10;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			return false;
		}
		bool loop_reduction()
		{
			int tbl[10]={0};
			std::stack<parse_unit> temp;
			for(int i=0;i<10;++i)
			{
				if(parser.empty())
					break;
				temp.push(parser.top());
				tbl[i]=temp.top().type;
				parser.pop();
			}
			for(int i=0;i<10;++i)
			{
				if(temp.empty())
					break;
				parser.push(temp.top());
				temp.pop();
			}
			if((tbl[2]==__while) && (tbl[1]==__scalar) && (tbl[0]==__statement))
			{
				parse_unit t;
				t.type=__loop;
				t.line=parser.top().line;
				for(int i=0;i<3;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[4]==__while) && (tbl[3]==__scalar) && (tbl[2]==__left_brace) && ((tbl[1]==__statement) || (tbl[1]==__statements)) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__loop;
				t.line=parser.top().line;
				for(int i=0;i<5;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if(((tbl[5]==__foreach) || (tbl[5]==__forindex)) && (tbl[4]==__left_curve) && (tbl[3]==__statement) && (tbl[2]==__identifier) && (tbl[1]==__right_curve) && (tbl[0]==__statement))
			{
				parse_unit t;
				t.type=__loop;
				t.line=parser.top().line;
				for(int i=0;i<6;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if(((tbl[7]==__foreach) || (tbl[7]==__forindex)) && (tbl[6]==__left_curve) && (tbl[5]==__statement) && (tbl[4]==__identifier) && (tbl[3]==__right_curve) && (tbl[2]==__left_brace) && ((tbl[1]==__statement) || (tbl[1]==__statements)) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__loop;
				t.line=parser.top().line;
				for(int i=0;i<8;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[7]==__for) && (tbl[6]==__left_curve) && (tbl[5]==__statements)
					&& (tbl[4]==__identifier)
					&& ((tbl[3]==__equal) || (tbl[3]==__add_equal) || (tbl[3]==__sub_equal) || (tbl[3]==__mul_equal) || (tbl[3]==__div_equal) || (tbl[3]==__link_equal) || (tbl[3]==__equal))
					&& ((tbl[2]==__identifier) || (tbl[2]==__scalar))
					&& (tbl[1]==__right_curve) && (tbl[0]==__statement))
			{
				parse_unit t;
				t.type=__loop;
				t.line=parser.top().line;
				for(int i=0;i<8;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[9]==__for) && (tbl[8]==__left_curve) && (tbl[7]==__statements)
			&& (tbl[6]==__identifier)
			&& ((tbl[5]==__equal) || (tbl[5]==__add_equal) || (tbl[5]==__sub_equal) || (tbl[5]==__mul_equal) || (tbl[5]==__div_equal) || (tbl[5]==__link_equal) || (tbl[5]==__equal))
			&& ((tbl[4]==__identifier) || (tbl[4]==__scalar))
			&& (tbl[3]==__right_curve) && (tbl[2]==__left_brace) && ((tbl[1]==__statement) || (tbl[1]==__statements)) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__loop;
				t.line=parser.top().line;
				for(int i=0;i<10;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			return false;
		}
		bool choose_reduction()
		{
			int tbl[8]={0};
			std::stack<parse_unit> temp;
			for(int i=0;i<8;++i)
			{
				if(parser.empty())
					break;
				temp.push(parser.top());
				tbl[i]=temp.top().type;
				parser.pop();
			}
			for(int i=0;i<8;++i)
			{
				if(temp.empty())
					break;
				parser.push(temp.top());
				temp.pop();
			}
			if(((tbl[2]==__if) || (tbl[2]==__elsif)) && (tbl[1]==__scalar) && (tbl[0]==__statement))
			{
				parse_unit t;
				t.type=__choose;
				t.line=parser.top().line;
				for(int i=0;i<3;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if(((tbl[4]==__if) || (tbl[4]==__elsif)) && (tbl[3]==__scalar) && (tbl[2]==__left_brace) && ((tbl[1]==__statement) || (tbl[1]==__statements)) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__choose;
				t.line=parser.top().line;
				for(int i=0;i<5;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[3]==__else) && (tbl[2]==__if) && (tbl[1]==__scalar) && (tbl[0]==__statement))
			{
				parse_unit t;
				t.type=__choose;
				t.line=parser.top().line;
				for(int i=0;i<4;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[5]==__else) && (tbl[4]==__if) && (tbl[3]==__scalar) && (tbl[2]==__left_brace) && ((tbl[1]==__statement) || (tbl[1]==__statements)) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__choose;
				t.line=parser.top().line;
				for(int i=0;i<6;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[1]==__else) && (tbl[0]==__statement))
			{
				parse_unit t;
				t.type=__choose;
				t.line=parser.top().line;
				for(int i=0;i<2;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[3]==__else) && (tbl[2]==__left_brace) && ((tbl[1]==__statement) || (tbl[1]==__statements)) && (tbl[0]==__right_brace))
			{
				parse_unit t;
				t.type=__choose;
				t.line=parser.top().line;
				for(int i=0;i<4;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			return false;
		}
		bool statement_check()
		{
			int tbl[3]={0};
			std::stack<parse_unit> temp;
			for(int i=0;i<3;++i)
			{
				if(parser.empty())
					break;
				temp.push(parser.top());
				tbl[i]=temp.top().type;
				parser.pop();
			}
			for(int i=0;i<3;++i)
			{
				if(temp.empty())
					break;
				parser.push(temp.top());
				temp.pop();
			}
			if((tbl[0]==__definition) || (tbl[0]==__assignment) || (tbl[0]==__function) || (tbl[0]==__loop) || (tbl[0]==__choose))
			{
				parse_unit t;
				t.type=__statement;
				t.line=parser.top().line;
				parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[2]==__return) && ((tbl[1]==__identifier) || (tbl[1]==__scalar)) && (tbl[0]==__semi))
			{
				parse_unit t;
				t.type=__statement;
				t.line=parser.top().line;
				for(int i=0;i<3;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			else if((tbl[2]!=__return) && ((tbl[1]==__identifier) || (tbl[1]==__continue) || (tbl[1]==__break) || (tbl[1]==__scalar)) && (tbl[0]==__semi))
			{
				parse_unit t;
				t.type=__statement;
				t.line=parser.top().line;
				for(int i=0;i<2;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			return false;
		}
		bool statements_reduction()
		{
			int tbl[2]={0};
			std::stack<parse_unit> temp;
			for(int i=0;i<2;++i)
			{
				if(parser.empty())
					break;
				temp.push(parser.top());
				tbl[i]=temp.top().type;
				parser.pop();
			}
			for(int i=0;i<2;++i)
			{
				if(temp.empty())
					break;
				parser.push(temp.top());
				temp.pop();
			}
			if(((tbl[1]==__statement) || (tbl[1]==__statements)) && (tbl[0]==__statement))
			{
				parse_unit t;
				t.type=__statements;
				t.line=parser.top().line;
				for(int i=0;i<2;++i)
					parser.pop();
				parser.push(t);
				return true;
			}
			return false;
		}
		void parse_process(std::list<token>& lexer)
		{
			stack_set_empty();
			for(std::list<token>::iterator i=lexer.begin();i!=lexer.end();++i)
			{
				parse_unit temp_parse;
				temp_parse.line=(*i).line;
				if(((*i).content=="var") || ((*i).content=="func") || ((*i).content=="return") || ((*i).content=="nil") || ((*i).content=="continue") || ((*i).content=="break") || ((*i).content=="and") || ((*i).content=="or"))
				{
					if((*i).content=="var")
						temp_parse.type=__var;
					else if((*i).content=="func")
						temp_parse.type=__func;
					else if((*i).content=="return")
						temp_parse.type=__return;
					else if((*i).content=="nil")
						temp_parse.type=__scalar;
					else if((*i).content=="continue")
						temp_parse.type=__continue;
					else if((*i).content=="break")
						temp_parse.type=__break;
					else if((*i).content=="and")
						temp_parse.type=__and_operator;
					else if((*i).content=="or")
						temp_parse.type=__or_operator;
				}
				else if((*i).type==IDENTIFIER)
				{
					temp_parse.type=__unknown_type_id;
				}
				else if(((*i).content=="for") || ((*i).content=="foreach") || ((*i).content=="while") || ((*i).content=="forindex"))
				{
					if((*i).content=="for")
						temp_parse.type=__for;
					else if((*i).content=="forindex")
						temp_parse.type=__forindex;
					else if((*i).content=="foreach")
						temp_parse.type=__foreach;
					else if((*i).content=="while")
						temp_parse.type=__while;
				}
				else if(((*i).content=="if") || ((*i).content=="else") || ((*i).content=="elsif"))
				{
					if((*i).content=="if")
						temp_parse.type=__if;
					else if((*i).content=="else")
						temp_parse.type=__else;
					else if((*i).content=="elsif")
						temp_parse.type=__elsif;
				}
				else if(((*i).content=="==") || ((*i).content=="!=") || ((*i).content==">") || ((*i).content==">=") || ((*i).content=="<") || ((*i).content=="<="))
				{
					if((*i).content=="==")
						temp_parse.type=__cmp_equal;
					else if((*i).content=="!=")
						temp_parse.type=__cmp_not_equal;
					else if((*i).content==">")
						temp_parse.type=__cmp_more;
					else if((*i).content==">=")
						temp_parse.type=__cmp_more_or_equal;
					else if((*i).content=="<")
						temp_parse.type=__cmp_less;
					else if((*i).content=="<=")
						temp_parse.type=__cmp_less_or_equal;
				}
				else if(((*i).content==";") || ((*i).content==",") || ((*i).content=="=") || ((*i).content==":") || ((*i).content=="."))
				{
					if((*i).content==";")
						temp_parse.type=__semi;
					else if((*i).content==",")
						temp_parse.type=__comma;
					else if((*i).content=="=")
						temp_parse.type=__equal;
					else if((*i).content==":")
						temp_parse.type=__colon;
					else if((*i).content==".")
						temp_parse.type=__dot;
				}
				else if(((*i).type==NUMBER) || ((*i).type==STRING))
				{
					temp_parse.type=__scalar;
				}
				else if(((*i).content=="+") || ((*i).content=="-") || ((*i).content=="*") || ((*i).content=="/") || ((*i).content=="~") || ((*i).content=="!"))
				{
					if((*i).content=="+")
						temp_parse.type=__add_operator;
					else if((*i).content=="-")
						temp_parse.type=__sub_operator;
					else if((*i).content=="*")
						temp_parse.type=__mul_operator;
					else if((*i).content=="/")
						temp_parse.type=__div_operator;
					else if((*i).content=="~")
						temp_parse.type=__link_operator;
					else if((*i).content=="!")
						temp_parse.type=__nor_operator;
				}
				else if(((*i).content=="+=") || ((*i).content=="-=") || ((*i).content=="*=") || ((*i).content=="/=") || ((*i).content=="~="))
				{
					if((*i).content=="+=")
						temp_parse.type=__add_equal;
					else if((*i).content=="-=")
						temp_parse.type=__sub_equal;
					else if((*i).content=="*=")
						temp_parse.type=__mul_equal;
					else if((*i).content=="/=")
						temp_parse.type=__div_equal;
					else if((*i).content=="~=")
						temp_parse.type=__link_equal;
				}
				else if(((*i).content=="(") || ((*i).content==")") || ((*i).content=="[") || ((*i).content=="]") || ((*i).content=="{") || ((*i).content=="}"))
				{
					char c=(*i).content[0];
					switch(c)
					{
						case '(':
							temp_parse.type=__left_curve;
							break;
						case ')':
							temp_parse.type=__right_curve;
							break;
						case '[':
							temp_parse.type=__left_bracket;
							break;
						case ']':
							temp_parse.type=__right_bracket;
							break;
						case '{':
							temp_parse.type=__left_brace;
							break;
						case '}':
							temp_parse.type=__right_brace;
							break;
					}
				}
				parser.push(temp_parse);//push this into stack
				
				bool reduction_complete=false;
				while(!reduction_complete)
				{
					if(scalar_check())
					{
						std::cout<<"line "<<parser.top().line<<": Special Scalar"<<std::endl;
						continue;
					}
					if(scalars_reduction())
					{
						std::cout<<"line "<<parser.top().line<<": Scalars"<<std::endl;
						continue;
					}
					if(identifier_check())
					{
						std::cout<<"line "<<parser.top().line<<": Identifier | Call function | Call hash | Call array"<<std::endl;
						continue;
					}
					if(identifiers_reduction())
					{
						std::cout<<"line "<<parser.top().line<<": Identifiers"<<std::endl;
						continue;
					}
					if(hashmember_check())
					{
						std::cout<<"line "<<parser.top().line<<": Hash member"<<std::endl;
						continue;
					}
					if(hashmembers_reduction())
					{
						std::cout<<"line "<<parser.top().line<<": Hash members"<<std::endl;
						continue;
					}
					if(calculation_reduction())
					{
						std::cout<<"line "<<parser.top().line<<": in Calculation"<<std::endl;
						continue;
					}
					if(loop_reduction())
					{
						std::cout<<"line "<<parser.top().line<<": Loop"<<std::endl;
						continue;
					}
					if(choose_reduction())
					{
						std::cout<<"line "<<parser.top().line<<": If-else"<<std::endl;
						continue;
					}
					if(definition_check())
					{
						std::cout<<"line "<<parser.top().line<<": Definition"<<std::endl;
						continue;
					}
					//assignment check must be put behind the definition check
					//because the assignment check has the same method to check
					//but assignment checks without a "var" so if you put the
					//assignment before definition,there may be a mistake.
					if(assignment_check())
					{
						std::cout<<"line "<<parser.top().line<<": Assignment"<<std::endl;
						continue;
					}
					if(function_def())
					{
						std::cout<<"line "<<parser.top().line<<": Definition for func()"<<std::endl;
						continue;
					}
					if(statement_check())
					{
						std::cout<<"line "<<parser.top().line<<": Statement"<<std::endl;
						if(statements_reduction())
							std::cout<<"line "<<parser.top().line<<": Statements"<<std::endl;
						continue;
					}
					reduction_complete=true;
				}
			}
			std::cout<<">>[Parser] Complete scanning."<<std::endl;
			return;
		}
		void parse_quiet_process(std::list<token>& lexer)
		{
			stack_set_empty();
			for(std::list<token>::iterator i=lexer.begin();i!=lexer.end();++i)
			{
				parse_unit temp_parse;
				temp_parse.line=(*i).line;
				if(((*i).content=="var") || ((*i).content=="func") || ((*i).content=="return") || ((*i).content=="nil") || ((*i).content=="continue") || ((*i).content=="break") || ((*i).content=="and") || ((*i).content=="or"))
				{
					if((*i).content=="var")
						temp_parse.type=__var;
					else if((*i).content=="func")
						temp_parse.type=__func;
					else if((*i).content=="return")
						temp_parse.type=__return;
					else if((*i).content=="nil")
						temp_parse.type=__scalar;
					else if((*i).content=="continue")
						temp_parse.type=__continue;
					else if((*i).content=="break")
						temp_parse.type=__break;
					else if((*i).content=="and")
						temp_parse.type=__and_operator;
					else if((*i).content=="or")
						temp_parse.type=__or_operator;
				}
				else if((*i).type==IDENTIFIER)
				{
					temp_parse.type=__unknown_type_id;
				}
				else if(((*i).content=="for") || ((*i).content=="foreach") || ((*i).content=="while") || ((*i).content=="forindex"))
				{
					if((*i).content=="for")
						temp_parse.type=__for;
					else if((*i).content=="forindex")
						temp_parse.type=__forindex;
					else if((*i).content=="foreach")
						temp_parse.type=__foreach;
					else if((*i).content=="while")
						temp_parse.type=__while;
				}
				else if(((*i).content=="if") || ((*i).content=="else") || ((*i).content=="elsif"))
				{
					if((*i).content=="if")
						temp_parse.type=__if;
					else if((*i).content=="else")
						temp_parse.type=__else;
					else if((*i).content=="elsif")
						temp_parse.type=__elsif;
				}
				else if(((*i).content=="==") || ((*i).content=="!=") || ((*i).content==">") || ((*i).content==">=") || ((*i).content=="<") || ((*i).content=="<="))
				{
					if((*i).content=="==")
						temp_parse.type=__cmp_equal;
					else if((*i).content=="!=")
						temp_parse.type=__cmp_not_equal;
					else if((*i).content==">")
						temp_parse.type=__cmp_more;
					else if((*i).content==">=")
						temp_parse.type=__cmp_more_or_equal;
					else if((*i).content=="<")
						temp_parse.type=__cmp_less;
					else if((*i).content=="<=")
						temp_parse.type=__cmp_less_or_equal;
				}
				else if(((*i).content==";") || ((*i).content==",") || ((*i).content=="=") || ((*i).content==":") || ((*i).content=="."))
				{
					if((*i).content==";")
						temp_parse.type=__semi;
					else if((*i).content==",")
						temp_parse.type=__comma;
					else if((*i).content=="=")
						temp_parse.type=__equal;
					else if((*i).content==":")
						temp_parse.type=__colon;
					else if((*i).content==".")
						temp_parse.type=__dot;
				}
				else if(((*i).type==NUMBER) || ((*i).type==STRING))
				{
					temp_parse.type=__scalar;
				}
				else if(((*i).content=="+") || ((*i).content=="-") || ((*i).content=="*") || ((*i).content=="/") || ((*i).content=="~") || ((*i).content=="!"))
				{
					if((*i).content=="+")
						temp_parse.type=__add_operator;
					else if((*i).content=="-")
						temp_parse.type=__sub_operator;
					else if((*i).content=="*")
						temp_parse.type=__mul_operator;
					else if((*i).content=="/")
						temp_parse.type=__div_operator;
					else if((*i).content=="~")
						temp_parse.type=__link_operator;
					else if((*i).content=="!")
						temp_parse.type=__nor_operator;
				}
				else if(((*i).content=="+=") || ((*i).content=="-=") || ((*i).content=="*=") || ((*i).content=="/=") || ((*i).content=="~="))
				{
					if((*i).content=="+=")
						temp_parse.type=__add_equal;
					else if((*i).content=="-=")
						temp_parse.type=__sub_equal;
					else if((*i).content=="*=")
						temp_parse.type=__mul_equal;
					else if((*i).content=="/=")
						temp_parse.type=__div_equal;
					else if((*i).content=="~=")
						temp_parse.type=__link_equal;
				}
				else if(((*i).content=="(") || ((*i).content==")") || ((*i).content=="[") || ((*i).content=="]") || ((*i).content=="{") || ((*i).content=="}"))
				{
					char c=(*i).content[0];
					switch(c)
					{
						case '(':
							temp_parse.type=__left_curve;
							break;
						case ')':
							temp_parse.type=__right_curve;
							break;
						case '[':
							temp_parse.type=__left_bracket;
							break;
						case ']':
							temp_parse.type=__right_bracket;
							break;
						case '{':
							temp_parse.type=__left_brace;
							break;
						case '}':
							temp_parse.type=__right_brace;
							break;
					}
				}
				parser.push(temp_parse);//push this into stack
				
				bool reduction_complete=false;
				while(!reduction_complete)
				{
					if(scalar_check())
						continue;
					if(scalars_reduction())
						continue;
					if(identifier_check())
						continue;
					if(identifiers_reduction())
						continue;
					if(hashmember_check())
						continue;
					if(hashmembers_reduction())
						continue;
					if(calculation_reduction())
						continue;
					if(loop_reduction())
						continue;
					if(choose_reduction())
						continue;
					if(definition_check())
						continue;
					//assignment check must be put behind the definition check
					//because the assignment check has the same method to check
					//but assignment checks without a "var" so if you put the
					//assignment before definition,there may be a mistake.
					if(assignment_check())
						continue;
					if(function_def())
						continue;
					if(statement_check())
					{
						if(statements_reduction())
							;
						continue;
					}
					reduction_complete=true;
				}
			}
			std::cout<<">>[Parser] Complete scanning."<<std::endl;
			return;
		}
		void print_error()
		{
			std::stack<parse_unit> temp_stack;
			while(!parser.empty())
			{
				if((parser.top().type!=__statement) && (parser.top().type!=__statements) && (parser.top().type!=__function) && (parser.top().type!=__definition) && (parser.top().type!=__assignment) && (parser.top().type!=__loop) && (parser.top().type!=__choose))
					temp_stack.push(parser.top());
				parser.pop();
			}
			if(!temp_stack.empty())
			{
				std::cout<<">>[Error] Parse error."<<std::endl;
				while(!temp_stack.empty())
				{
					int l=temp_stack.top().line;
					std::cout<<"line "<<l<<": ";
					while(l==temp_stack.top().line)
					{
						print_token_type(temp_stack.top().type);
						std::cout<<" ";
						temp_stack.pop();
						if(temp_stack.empty())
							break;
					}
					std::cout<<std::endl;
				}
			}
			else
				std::cout<<">>[Parser] Parse analysis complete."<<std::endl;
			return;
		}
		
};

#endif
