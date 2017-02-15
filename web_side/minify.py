#!/usr/bin/env python
import json
import sys
import urllib
import urllib2

if __name__=='__main__':
	try:
		with open('index.html') as html:
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
			with open('../firmware/webpage.h','w+') as c_file:
				c_file.write('const char webpage[] PROGMEM='+c_code.rstrip()+';')
				c_file.close()
	except KeyboardInterrupt:
		exit(1)
	except Exception as error:
		print(error)
		exit(1)
	except:
		print('Unknown error occurred.')
		exit(1)