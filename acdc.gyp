{
	'variables': {
		'default_cflags': [
			'-pthread',
			'-Wall',
			'-Werror',
			'-m64',
			'-mcx16',
		],
	},
    'target_defaults': {
	'configurations': {
		'Debug': {
			'cflags': ['<@(default_cflags)', '-g', '-O0', '-gdwarf-2'],
            'ldflags': ['-pthread', '-lmmtk', '-Lallocators/'],
		},
		'Release': {
			'cflags': ['<@(default_cflags)', '-O3'],
			'ldflags': ['-pthread', '-lmmtk', '-Lallocators/'],
		},
	},
    },
    'targets': [
        {
            'target_name': 'acdc',
            'type': 'executable',
            'defines': [],
            'include_dirs': [
                'src',
            ],
            'libraries': ['allocators/libmmtk.so'],
            'sources': [
                'src/acdc.c',
                'src/lifetime-class.c',
                'src/memory.c',
                'src/barrier.c',
                'src/osx-barrier.c',
                'src/lifetime-size-classes.c',
                'src/metadata-allocator.c',
                'src/distribution.c',
                'src/main.c',
                'src/proc-status.c',
                'src/alloc/nulloc.c',
            ],

        }

    ],
}
