tool 'bglgen' {
   app {
      icon 'icon/bengine-warm.ico',
      custom {
         outputs = 'src/lexer.cpp',
         inputs = 'meta/lexer.rl',
         rule = 'ragel',
         command = 'ragel $flags -o $out $in',
         flags = '-s -C -L -G2',
         generator = true,
         restat = true
      },
      link_project {
         'core',
         'core-id-with-names',
         'util',
         'util-fs',
         'util-string',
         'sqlite',
         'cli',
         'blt'
      }
   }
}
