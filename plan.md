
<font size="5">

- [+] c[i]   ->    element { i, &c }
    
    c[i] = c[j]   ->   element { i, &c } = element { j, &c }   ->   c.get_history().add(set(i, j))
    
    c[i] > c[j]   ->   element { i, &c } > element { j, &c }   ->   c.get_history().add(compare(i, j))
- [+] add history include
- [+] add action wrapper include
- [ ] add add/sub/mul/div for element
- [ ] add concept for all structures
- [ ] add algorithm from function?/refactor algorithm structure
- [ ] add Lua/Python script
- [ ] add iterator? add wrap for std container??
- [ ] add Qt???
</font>


c[i] -> element { i, &c }
c[i]->rot(30) -> element { i, &c } -> element_method_wrapper { element { i, &c } } -> rot(30)
                                        c[i].rot(i, 30)