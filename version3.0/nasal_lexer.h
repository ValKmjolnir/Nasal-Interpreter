#ifndef __NASAL_LEXER_H__
#define __NASAL_LEXER_H__

#define IS_IDENTIFIER_HEAD(c) (c=='_')||('a'<=c && c<='z')||('A'<=c&&c<='Z')
#define IS_IDENTIFIER_BODY(c) (c=='_')||('a'<=c && c<='z')||('A'<=c&&c<='Z')||('0'<=c&&c<='9')
#define IS_NUMBER_HEAD(c)     ('0'<=c&&c<='9')
#define IS_NUMBER_BODY(c)     ('0'<=c&&c<='9')||('a'<=c&&c<='f')||('A'<=c&&c<='F')||(c=='e'||c=='E'||c=='.'||c=='x'||c=='o')
#define IS_STRING_HEAD(c)     (c=='\''||c=='\"')
// single operators have only one character
#define IS_SINGLE_OPRATOR(c)  (c=='('||c==')'||c=='['||c==']'||c=='{'||c=='}'||c==','||c==';'||c=='|'||c==':'||\
							   c=='?'||c=='`'||c=='&'||c=='@'||c=='%'||c=='$'||c=='^'||c=='\\')
#define IS_DOT(c)             (c=='.')
// calculation operators may have two chars, for example: += -= *= /= ~= != == >= <=
#define IS_CALC_OPERATOR(c)   (c=='='||c=='+'||c=='-'||c=='*'||c=='!'||c=='/'||c=='<'||c=='>'||c=='~')
#define IS_NOTE_HEAD(c)       (c=='#')

#ifndef TOKEN_TABLE_SIZE
#define TOKEN_TABLE_SIZE 44
struct token_table
{
    std::string str;
    int tok_type;
}tok_tbl[TOKEN_TABLE_SIZE]=
{
    {"for"     ,tok_for          },
    {"forindex",tok_forindex     },
    {"foreach" ,tok_foreach      },
    {"while"   ,tok_while        },
    {"var"     ,tok_var          },
    {"func"    ,tok_func         },
    {"break"   ,tok_break        },
    {"continue",tok_continue     },
    {"return"  ,tok_return       },
    {"if"      ,tok_if           },
    {"elsif"   ,tok_elsif        },
    {"else"    ,tok_else         },
    {"nil"     ,tok_nil          },
    {"("       ,tok_left_curve   },
    {")"       ,tok_right_curve  },
    {"["       ,tok_left_bracket },
    {"]"       ,tok_right_bracket},
    {"{"       ,tok_left_brace   },
    {"}"       ,tok_right_brace  },
    {";"       ,tok_semi         },
    {"and"     ,tok_and          },
    {"or"      ,tok_or           },
    {","       ,tok_comma        },
    {"."       ,tok_dot          },
    {"..."     ,tok_ellipsis     },
    {"?"       ,tok_quesmark     },
    {":"       ,tok_colon        },
    {"+"       ,tok_add          },
    {"-"       ,tok_sub          },
    {"*"       ,tok_mult         },
    {"/"       ,tok_div          },
    {"~"       ,tok_link         },
    {"!"       ,tok_not          },
    {"+="      ,tok_add_equal    },
    {"-="      ,tok_sub_equal    },
    {"*="      ,tok_mult_equal   },
    {"/="      ,tok_div_equal    },
    {"~="      ,tok_link_equal   },
    {"=="      ,tok_cmp_equal    },
    {"!="      ,tok_cmp_not_equal},
    {"<"       ,tok_less_than    },
    {">"       ,tok_greater_than },
    {"<="      ,tok_less_equal   },
    {">="      ,tok_greater_equal},
};
#endif

struct token
{
    int line;
    int type;
    std::string str;
};

class nasal_lexer
{
private:
    std::vector<token> token_list;
public:
    void delete_tokens();
    void scanner(std::vector<char>&);
};

void nasal_lexer::delete_tokens()
{
    token_list.clear();
    return;
}

void nasal_lexer::scanner(std::vector<char>& res)
{
    return;
}

#endif