import datetime
import markdown


kHeader = '''<!DOCTYPE HTML>

<html>
<head>
   <title>{0}</title>
   <link href="tetrakite.css" type="text/css" rel="stylesheet" />
</head>
<body>
<div class="content">
'''

kFooter = '''<p><i>File version {0}</i></p></div></body>
</html>
'''


import sys
fileBase = sys.argv[1]


with open('{0}.md'.format(fileBase), 'r') as f:
   txt = f.read()
   txt = txt.decode("utf-8")
   with open('{0}.html'.format(fileBase), "wt") as outFile:
      outFile.write(kHeader.format(fileBase))
      outFile.write(markdown.markdown(txt))
      outFile.write(kFooter.format(datetime.date.today()))

      
