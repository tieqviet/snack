# snack

Scripting language that uses LLVM as backend, compiled code, JITing or Intepretering. 
Has the ability to generate fully dynamic reflection by a post-precessor

![Design]()


Example

```
#Snack Programming Language
#Copyright (C) 2017 dotrongthu
#Copyright (C) 2017 LLVM Project

using 
	std,
	math;

$def a(x) std::cout << x

#Basic

#Type specifed with the formula: *var* as *type* = *intialize_val*; 

str_ha = 'string'           #When there is no colon expected, the parser intended it's one line only. Any statement expect comment will be marked as error
is_true = 2 <> 5.4;   #When there is colon, many statements can be inserted. Statements after a comment will be considered as comments

func max with a as int, b as int: int; 
bg
	if a > b 
	bg
		ret a;
	end
	elif b > a
	bg
		ret b;
	end

	ret 0;
end

func check_type: none;
bg
end

type integer as u32;

struct p5 with
	a as integer;
	b as integer*;
	c as penable;
end

class p6 with
bg
	public a as integer;

	public 
	bg
		a as block;
		b as p5;
	end
		
	public lol123 as integer 
	bg
		get bg ret a; end;
		set bg a = value; end;
	end
end

a("Hi, I'm a scirpt!");

$undef a

a(test);
```