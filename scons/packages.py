import shutil
import tempfile
import urllib.request
from urllib.parse import urljoin
import re
import zipfile
from html.parser import HTMLParser
import os
from tqdm import tqdm
import os.path


def ResolveDownloadUrl(downloadUrl, pattern):
    with urllib.request.urlopen(downloadUrl ) as response:
       html = response.read().decode("utf-8")
    result = []
    class MyHTMLParser(HTMLParser):
        def handle_starttag(self, tag, attrs):
            if tag == 'a':
                for attr in attrs:
                    if attr[0] == 'href' and re.match(pattern, attr[1]):
                        result.append(urljoin(downloadUrl, attr[1]))
        def handle_endtag(self, tag):
            pass
        def handle_data(self, data):
            pass

    parser = MyHTMLParser()
    parser.feed(html)
    print (result)
    return result


def DownloadUrl(url, output_path = None):
    class DownloadProgressBar(tqdm):
        def update_to(self, b=1, bsize=1, tsize=None):
            if tsize is not None:
                self.total = tsize
            self.update(b * bsize - self.n)
    if output_path is None:
        output_path = tempfile.mktemp()
    print('Downloading "%s" to "%s"' % (url, output_path))
    with DownloadProgressBar(unit='B', unit_scale=True,
                             miniters=1, desc=url.split('/')[-1]) as t:
        urllib.request.urlretrieve(url, filename=output_path, reporthook=t.update_to)
    return output_path
        

def Unpack(file, toPath):
    print('Extracting "%s" to "%s"' % (file, toPath)) 
    with zipfile.ZipFile(file, 'r') as zip_ref:
        zip_ref.extractall(toPath)


def DownloadPackage(env, package):
    home = os.path.expanduser("~")
    packageExtractPath = os.path.join(home, '.mcucpp', package['type'], package['name'])
    if 'verifyPath' in package:
        verifyPath = os.path.join(packageExtractPath, package['verifyPath'])
        if os.path.isfile(verifyPath):
            print ('Package "%s" is already in place' % package['name'])
            return
            
    if 'searchUrl' in package:
        if 'searchPattern' in package:
            searchPattern = package['searchPattern']
        else:
            searchPattern = r'.*\.zip'
        downloadUrl = ResolveDownloadUrl(package['searchUrl'], searchPattern)[0]
    else:
        downloadUrl = package['dpwnloadUrl']
    file = DownloadUrl(downloadUrl)
    
    Unpack(file, packageExtractPath)
    env[package['name']] = packageExtractPath
    os.remove(file)
