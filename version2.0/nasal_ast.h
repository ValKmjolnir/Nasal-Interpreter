#ifndef __NASAL_AST_H__
#define __NASAL_AST_H__

class abstract_syntax_tree
{
	private:
		// basic elements
		int line;
		int node_type;
		std::list<abstract_syntax_tree> children;

		// is this node is a number|string|identifier node
		// then the three elements below is of great use
		// var_name is set for __id
		double      var_number;
		std::string var_string;
		std::string var_name;
	public:
		/* basic */
		abstract_syntax_tree();
		abstract_syntax_tree(const abstract_syntax_tree&);
		~abstract_syntax_tree();
		abstract_syntax_tree& operator=(const abstract_syntax_tree&);
		
		/* main functions */
		// print
		void print_tree();
		void print_tree_block(const int);

		// set
		void set_clear();
		void set_node_line(const int);
		void set_node_type(const int);
		void set_var_number(std::string);
		void set_var_string(std::string);
		void set_var_name(std::string);
		void add_children(abstract_syntax_tree);

		// get
		int         get_node_line();
		int         get_node_type();
		double      get_var_number();
		std::string get_var_string();
		std::string get_var_name();
		std::list<abstract_syntax_tree>& get_children();

		// merge
		void merge_children(abstract_syntax_tree&);
};

abstract_syntax_tree::abstract_syntax_tree()
{
	node_type=__null_type;
	line=0;
	var_number=0;
	var_string="";
	var_name="";
	children.clear();
	return;
}

abstract_syntax_tree::abstract_syntax_tree(const abstract_syntax_tree& tmp)
{
	node_type=tmp.node_type;
	line=tmp.line;
	var_number=tmp.var_number;
	var_string=tmp.var_string;
	var_name=tmp.var_name;
	children=tmp.children;
	return;
}

abstract_syntax_tree::~abstract_syntax_tree()
{
	node_type=__null_type;
	line=0;
	var_number=0;
	var_string.clear();
	var_name.clear();
	children.clear();
	return;
}

abstract_syntax_tree& abstract_syntax_tree::operator=(const abstract_syntax_tree& tmp)
{
	node_type=tmp.node_type;
	line=tmp.line;
	var_number=tmp.var_number;
	var_string=tmp.var_string;
	var_name=tmp.var_name;
	children.clear();
	children=tmp.children;
	return *this;
}

void abstract_syntax_tree::print_tree()
{
	std::cout<<">> [Abstract-syntax-tree] get tree root: "<<(this)<<""<<std::endl;
	print_tree_block(1);
	return;
}

void abstract_syntax_tree::print_tree_block(const int n)
{
	std::string __str="";
	for(int i=0;i<n;++i)
		__str+="| ";
	std::cout<<__str;
	print_ast_type(node_type);
	switch(node_type)
	{
		case __number:  std::cout<<": "<<var_number;break;
		case __string:  std::cout<<": "<<var_string;break;
		case __id:
		case __dynamic_id:
		case __call_hash: std::cout<<": "<<var_name;break;
		case __call_vector:
		case __call_function: break;
		default: break;
	}
	std::cout<<std::endl;
	if(!children.empty())
	{
		for(std::list<abstract_syntax_tree>::iterator i=children.begin();i!=children.end();++i)
			i->print_tree_block(n+1);
	}
	return;
}

void abstract_syntax_tree::set_clear()
{
	node_type=__null_type;
	line=0;
	var_number=0;
	var_string="";
	var_name="";
	children.clear();
	return;
}

void abstract_syntax_tree::set_node_type(const int __node_type)
{
	node_type=__node_type;
	return;
}

void abstract_syntax_tree::set_node_line(const int __line)
{
	if(__line>=0)
		line=__line;
	else
	{
		std::cout<<">> [Abstract-syntax-tree-warning] incorrect line under 0: "<<__line<<"."<<std::endl;
		line=0;
	}
	return;
}

void abstract_syntax_tree::set_var_string(std::string __str)
{
	var_string=__str;
	return;
}

void abstract_syntax_tree::set_var_number(std::string __str)
{
	var_number=trans_string_to_number(__str);
	return;
}

void abstract_syntax_tree::set_var_name(std::string __str)
{
	var_name=__str;
	return;
}

void abstract_syntax_tree::add_children(abstract_syntax_tree p)
{
	// use abstract_syntax_tree instead of abstract_syntax_tree&
	// because when this function get a 'p' from returned value of
	// another function,this may result in s SIGSEGV.
	children.push_back(p);
	return;
}

int abstract_syntax_tree::get_node_type()
{
	return node_type;
}

int abstract_syntax_tree::get_node_line()
{
	return line;
}

double abstract_syntax_tree::get_var_number()
{
	return var_number;
}

std::string abstract_syntax_tree::get_var_string()
{
	return var_string;
}

std::string abstract_syntax_tree::get_var_name()
{
	return var_name;
}

std::list<abstract_syntax_tree>& abstract_syntax_tree::get_children()
{
	return children;
}

void abstract_syntax_tree::merge_children(abstract_syntax_tree& tmp)
{
	for(std::list<abstract_syntax_tree>::iterator i=tmp.children.begin();i!=tmp.children.end();++i)
		this->children.push_back(*i);
	return;
}

#endif
/*
examples of each type of sub-tree

ast begins in root node:
	root
		statement_1
		statement_2
...

source code:
0xdeadbeef;
'str';
"str";
ast:
	root
		num: 3.73593e+009
		str: str
		str: str

source code:
[0,1,2,3][0:2];
ast:
	root
		vector:
			num: 0
			num: 1
			num: 2
			num: 3
			call_vector
				sub_vector
					num: 0
					num: 2

source code:
{hello:"world"}.hello;
ast:
	root
		hash
			hash_member
				id: hello
				str: world
			call_hash: hello

source code:
1+2*(1-3)/4~'str';
ast:
	root
		~
			+
			num: 1
			/
				*
				num: 2
				-
					num: 1
					num: 3
				num:4
			str: str

source code:
var a=cd? 1:0;
ast:
	root
		definition
			id: a
			?
				id: cd
				num: 1
				num: 0

source code:
var h={a:1,b:2,c:'3',};
ast:
	root
		definition
			id: h
			hash
				hash_member
					id: a
					num: 1
				hash_member
					id: b
					num: 2
				hash_member
					id: c
					str: 3

source code:
var id=1;
ast:
	root
		definition
			id: id
			num: 1

source code:
var (id1,id2,id3)=(1,2,3);
(var id1,id2,id3)=(1,2,3);
ast:
	root
		definition
			identifiers
				id: id1
				id: id2
				id: id3
			scalars
				num: 1
				num: 2
				num: 3

source code:
var (id1,id2,id3)=[1,2,3];
(var id1,id2,id3)=[1,2,3];
ast:
	root
		definition
			identifiers
				id: id1
				id: id2
				id: id3
			vector
				num: 1
				num: 2
				num: 3

source code:
(id1,id2,id3)=(1,2,3);
ast:
	root
		=
			scalars
				id: id1
				id: id2
				id: id3
			scalars
				num: 1
				num: 2
				num: 3

source code:
(id1[0],id2,id3)=[1,2,3];
ast:
	root
		=
			scalars
				id: id1
					call_vector
						num: 0
				id: id2
				id: id3
			vector
				num: 1
				num: 2
				num: 3

source code:
id.call_hs(call_f)[call_vec][subvec1:subvec2,subvec3];
ast:
	root
		id: id
			call_hash: call_hs
			call_function
				id: call_f
			call_vector
				id: call_vec
			call_vector
				sub_vector
					id: subvec1
					id: subvec2
				id: subvec3

source code:
id.id.id.id.id.id.id.id;
ast:
	root
		id: id
			call_hash: id
			call_hash: id
			call_hash: id
			call_hash: id
			call_hash: id
			call_hash: id
			call_hash: id

source code:
function(a,b,c,d,e);
function(func a);
function(func a());
ast:
	root
		id: function
			call_function
				id: a
				id: b
				id: c
				id: d
				id: e
		id: function
			call_function
				id: a
		id: function
			call_function
				id: a
					call_function

source code:
function(
	a,
	b,
	func{print("hello");}
);
ast:
	id: function
		call_function
			id: a
			id: b
			function
				parameters
				block
					id: print
						call_function
							str: hello

source code:
function(a:1,b:2,c:3,);
ast:
	root
		id: function
			call_function
				special_parameter
					id: a
					num: 1
				special_parameter
					id: b
					num: 2
				special_parameter
					id: c
					num: 3

source code:
(func{print(1);})();
ast:
	root
		function
			parameters
			block
				id: print
					call_function
						num: 1
			call_function

source code:
while(id)
{
	fun();
	var a=1;
	var b=2;
}
ast:
	root
		while
			id: id
			block
				id: func
					call_function
				definition
					id: 1
					num: 1
				definition
					id: b
					num: 2

source code:
for(;;){}
ast:
	root
		for
			null_type
			null_type
			null_type
			block

source code:
for(var i=1;i<100;i+=1){}
ast:
	root
		for
			definition
				id: i
				num: 1
			<
				id: i
				num: 100
			+=
				id: i
				num: 1
			block

source code:
foreach(var i;[0,1,2]){}
forindex(var i;[0,1,2]){}
ast:
	root
		foreach
			id: i
			vector
				num: 0
				num: 1
				num: 2
			block
		forindex
			id: i
			vector
				num: 0
				num: 1
				num: 2
			block

source code:
if(condition_1)
{
}
else if(condition_2)
{
}
else
{
}
ast:
	root
		conditional
			if
				id: condition_1
				block
			else
				block
					if
						id: condition_2
						block
					else
						block

source code:
if(condition_1)
{
}
elsif(condition_2)
{
}
elsif(condition_3)
{
}
ast:
	root
		conditional
			if
				id: condition_1
				block
			elsif
				id: condition_2
				block
			elsif
				id: condition_3
				block

source code:
var function=func{};
ast:
	root
		definition
			id: function
			function
				parameters
				block

source code:
var function=func(x,y,dyn...){};
ast:
	root
		definition
			id: function
			function
				parameters
					id: x
					id: y
					id...: dyn
				block

source code:
var function=func(x=2,y=1){};
ast:
	root
		definition
			id: function
			function
				parameters
					default_parameter
						id: x
						num: 2
					default_parameter
						id: y
						num: 1
					block

source code:
var function=func(args...)
{
	if(size(args)<3)
	{
		print("error");
		return 0;
	}
	if(args[0]==args[1] or args[1]==args[2])
	{
		return args[0]*args[1]*args[2];
	}
	elsif(args[2]>args[1])
	{
		return -1;
	}
	for(var i=0;i<size(args);i+=1)
	{
		if(args[i]>100)
			break;
		else if(args[i]<0)
			return args[i];
	}
	return 0;
}
ast:
	root
		definition
			id: function
			function
				parameters
					id...: args
				block
					conditional
						if
							<
								id: size
									call_function
										id: args
								num: 3
							block
								id: print
									call_function
										str: error
								return
									num: 0
					conditional
						if
							or
								==
									id: args
										call_vector
											num: 0
									id: args
										call_vector
											num: 1
								==
									id: args
										call_vector
											num: 1
									id: args
										call_vector
											num: 2
							block
								return
									*
										*
											id: args
												call_vector
													num: 0
											id: args
												call_vector
													num: 1
										id: args
											call_vector
												num: 2
						elsif
							>
								id: args
									call_vector
										num: 2
								id: args
									call_vector
										num: 1
							block
								return
									-
										num: 0
										num: 1
					for
						definition
							id: i
							num: 0
						<
							id: i
							id: size
								call_function
									id: args
						+=
							id: i
							num: 1
						block
							conditional
								if
									>
										id: args
											call_vector
												id: i
										num: 100
									block
										break
								else
									block
										conditional
											if
												<
													id: args
														call_vector
															id: i
													num: 0
												block
													return
														id: args
															call_vector
																id: i
					return
						num: 0
*/