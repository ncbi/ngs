from setuptools import setup
#import version

#ver = version.get_git_version()

setup(name='ngs',
      #version=ver,
      version = "1.0",
      author='sra-toolkit',
      author_email='ukrainch@ncbi.nlm.nih.gov',
      packages=['ngs'],
      include_package_data=True,
      scripts=[],
      #test_suite="tests",
      )
