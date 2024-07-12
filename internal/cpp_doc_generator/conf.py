import subprocess, os

project = 'iceoryx2'
copyright = '2024, ekxide IO GmbH'
author = 'ekxide IO GmbH'

subprocess.call('mkdir -p target/doxygen_docs', shell=True)
subprocess.call('doxygen -g Doxyfile.global', shell=True)
subprocess.call('doxygen Doxyfile', shell=True)

extensions = [
    'breathe',
    'exhale',
]

# -- Exhale configuration ---------------------------------------------------
# Setup the breathe extension
breathe_projects = {
    "iceoryx2": "./target/doxygen_docs/xml"
}
breathe_default_project = "iceoryx2"

 # Setup the exhale extension
exhale_args = {
    # These arguments are required
    "containmentFolder":     "./target/api",
    "rootFileName":          "library_root.rst",
    "rootFileTitle":         "iceoryx2",
    "doxygenStripFromPath":  "..",
    # Suggested optional arguments
    "createTreeView":        True,
    # TIP: if using the sphinx-bootstrap-theme, you need
    # "treeViewIsBootstrap": True,
}

# Tell sphinx what the primary language being documented is.
primary_domain = 'cpp'

# Tell sphinx what the pygments highlight language should be.
highlight_language = 'cpp'

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    'canonical_url': '',
    'analytics_id': '',
    'display_version': True,
    'prev_next_buttons_location': 'bottom',
    'style_external_links': False,

    'logo_only': False,

    # Toc options
    'collapse_navigation': True,
    'sticky_navigation': True,
    'navigation_depth': 4,
    'includehidden': True,
    'titles_only': False
}
