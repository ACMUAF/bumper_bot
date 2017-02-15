#!/usr/bin/env python
import json
import sys
import urllib
import urllib2

if __name__=='__main__':
	if len(sys.argv)<2:
		print('Expected filename')
		exit(1)
	try:
		with open(sys.argv[1]) as html:
			code=html.read()
			req=urllib2.Request('http://minify.minifier.org/','source='+urllib.quote_plus(code)+'&type=js')
			minified=json.loads(urllib2.urlopen(req).read())['minified'].replace('"','\\"')
			c_code=''
			first_line=True
			for line in minified.split('\n'):
				if not first_line:
					c_code+='\t'
				first_line=False
				c_code+='"'+line+'"\n'
			print('const char webpage[] PROGMEM='+c_code.rstrip()+';')
	except KeyboardInterrupt:
		exit(1)
	except Exception as error:
		print(error)
		exit(1)
	except:
		print('Unknown error occurred.')
		exit(1)