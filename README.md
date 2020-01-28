# *LEMIN*
'School 42' project. Optimized graph and minimum-cost flow (disjoint path finding).

[This is the subject.](/docs/lem-in.en.pdf)

### More detailed description [posted on wiki](https://github.com/elijahkash/lemin/wiki)

## Foreword

- Project based on my [libft](https://github.com/elijahkash/libft)
- Be welcome to use my project, `clone` and `fork` it!
- If you find a bug, please, create new `issue` (or contact with me in any way you want) to report about it! I will be glad to fix it! If you find a mistake in my spelling (because I'm from Russia, and English is not my native language), please notify me too!
- If you `clone/fork` this lib and realise some new feature, you can create `pull request`! I am definitely interested in this!
- Also, I will be happy if you `follow` me, or `star` this project, ecpecially if you find it usefull, or use my code!
- For all questions be welcome to contact me in any way you want 👋
> Here is my  mail: _elijahkash.code@gmail.com_

## How to use:

#### 1. Compile

- run `make` or `make lem-in` to compile `lem-in`
- run `make clean` to delete tmp files after compile (only `lem-in` will be remain)
- run `make fclean` to delete all files after compile
- run `make cmake` if you change any filename.c/h to update the configuration of build (and include new file)
- run `make re` to recompile all files

> _There is no real need to do this! If you change a file and run `make`, the project will recompile this file, as well as all files that depend on this file and relink the lem-in!_

#### 2. Debug

If you run `make` or `make lem-in`, the compiler will compile the project with the `-O3 -Wall -Wextra -Werror` flags for better code performance and strict error checking!
But if you want to debug some code, you probably want to ignore warnings and be able to use a debugger (lldb, gdb, etc.).
In that case
- run `make debug`

`lem-in.debug` will be created, and compiler flags now will be `-g3 -Wall -Wextra`.

#### 3. Using

Example of using:
 - `./lem-in < ./test_maps/test_spec
 
For easy cheking of result use `grep`:
 - `./lem-in < ./test_maps/pylone_400_10_10_35_35_3_no_z | grep '#>'`
 
Also you may use map generator (provided by _Ecole42_):
 - `./test_maps/generator --big-superposition | ./lem-in | grep -E '#>|#Here'`
 
For real nice and useful visualisation look [here](https://github.com/SemRunner/lem-in) - my friend's @SemRunner implementation

## NORMINETTE

If you are part of the **_42_** society, you now all about the *_norminette_* and you may skip this part.
Otherwise, you may discover some strange `code` and /file_structure in this project.
This is becouse all project in **_42_** must be written in [accordance with the NORM](/docs/norme.en.pdf).

Here are the point that most affect the code:
- You cannot have more than 5 function-definitions in a `.c` file.

We have programm called `norminette`. It check every project. If it found an error, project considered as **FAIL**

## License
- See the [LICENSE](./LICENSE) file for license rights and limitations (MIT).
- All rights to files in [/docs](/docs) belong to [ECOLE 42](https://www.42.fr/)
