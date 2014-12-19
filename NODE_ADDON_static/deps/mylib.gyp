{
    "targets": [
        {
            "target_name": "mylib",
            "product_name": "mylib",
            "type": "static_library",
            "sources": [
                "implementation.cpp"
            ],
            "include_dirs": [
                "include"
            ],
            'direct_dependent_settings': {
              'include_dirs': [ 'include/' ],
            }
        }
    ]
}
