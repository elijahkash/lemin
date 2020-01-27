# *LEMIN*
'School 42' project. Optimized graph and minimum-cost flow.

### More detailed description [posted on wiki](https://github.com/elijahkash/lemin/wiki) (in work now)

## Foreword

- Be welcome to use my project, `clone` and `fork` it!
- If you find a bug, please, create new `issue` (or contact with me in any way you want) to report about it! I will be glad to fix it! If you find a mistake in my spelling (because I'm from Russia, and English is not my native language), please notify me too!
- If you `clone/fork` this lib and realise some new feature, you can create `pull request`! I am definitely interested in this!
- Also, I will be happy if you `follow` me, or `star` this project, ecpecially if you find it usefull, or use my code!
- For all questions be welcome to contact me in any way you want 👋
> Here is my  mail: _elijahkash.code@gmail.com_

## How to use:

#### 1. Compile

- run `make` or `make libft` to compile `libft.a`
- run `make clean` to delete tmp files after compile (only `libft.a` will be remain)
- run `make fclean` to delete all files after compile
- run `make cmake` if you change any filename.c/h to update the configuration of build (and include new file)
- run `make re` to recompile all files

> _There is no real need to do this! If you change a file and run `make`, the project will recompile this file, as well as all files that depend on this file and relink the lib!_

## NORMINETTE

If you are part of the **_42_** society, you now all about the *_norminette_* and you may skip this part.
Otherwise, you may discover some strange `code` and /file_structure in this project.
This is becouse all project in **_42_** must be written in [accordance with the NORM](/docs/norme.en.pdf).

Here are the point that most affect the code:
- You cannot have more than 5 function-definitions in a `.c` file.

We have programm called `norminette`. It check every project. If it found an error, project considered as **FAIL**

## License
- See the [LICENSE](./LICENSE) file for license rights and limitations (MIT).
