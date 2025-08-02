# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os
from sphinx.directives.code import LiteralInclude

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'chesscxx'
copyright = '2025, igorqs'
author = 'igorqs'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [ "breathe", "sphinx_inline_tabs", "myst_parser" ]
templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_copy_source = False
html_show_sourcelink = False
html_theme = 'furo'

# -- Breathe Configuration ---------------------------------------------------

breathe_default_project = "chesscxx"

# -- Custom Sphinx directives for include examples ---------------------------

_examples_source_dir = f"/{os.environ.get('EXAMPLES_RELATIVE_SOURCE_DIR', '../examples')}"
_examples_binary_dir = f"/{os.environ.get('EXAMPLES_RELATIVE_BINARY_DIR', '../build/examples')}"

class IncludeExampleOutput(LiteralInclude):
    def run(self):
        self.arguments[0] = f"{_examples_binary_dir}/{self.arguments[0]}.out"
        return super().run()

class IncludeExampleSource(LiteralInclude):
    def run(self):
        self.arguments[0] = f"{_examples_source_dir}/{self.arguments[0]}.cpp"
        return super().run()

class IncludeExampleData(LiteralInclude):
    def run(self):
        self.arguments[0] = f"{_examples_source_dir}/{self.arguments[0]}"
        return super().run()

def setup(app):
    app.add_directive("includeexampleoutput", IncludeExampleOutput)
    app.add_directive("includeexamplesource", IncludeExampleSource)
    app.add_directive("includeexampledata", IncludeExampleData)
