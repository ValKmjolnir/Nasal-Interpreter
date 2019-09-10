#ifndef __AST_GENERATOR_H__
#define __AST_GENERATOR_H__

class ast_generator
{
	private:
		ast_tree_node root;
		std::stack<parse_unit> parse;
	public:
		ast_generator()
		{
			;
		}
		void input_token_stack(std::stack<parse_unit>& temp)
		{
			parse=temp;
			return;
		}
		void gen_main_prog()
		{
			return;
		}
		void run()
		{
			return;
		}
};

#endif
