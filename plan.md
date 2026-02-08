

c[i]   ->    set_wrap { i, &c }

c[i] = c[j]   ->   set_wrap { i, &c } = set_wrap { j, &c }   ->   c.get_history().add(set_wrap(i, j))

c[i] > c[j]   ->   set_wrap { i, &c } > set_wrap { j, &c }   ->   c.get_history().add(compare_wrap(i, j))


