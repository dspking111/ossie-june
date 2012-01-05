## Copyright 2005, 2006, 2007, 2008 Virginia Polytechnic Institute and State University
##
## This file is part of the OSSIE ALF Waveform Application Visualization Environment
##
## ALF is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## ALF is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with OSSIE Waveform Developer; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import os, sys
import xml.dom.minidom
from xml.dom.minidom import Node
import wx
try:  # mac
    import importIDL
except ImportError:
    import WaveDev.wavedev.importIDL as importIDL

class Tool:
    def __init__ (self):
        self.name = ''
        self.startfile = ''
        self.modulename = ''
        self.packagename = ''
        self.interface = []
        self.module = None
        
class ToolInterface:
    def __init__ (self):
        self.name = ''
        self.namespace = ''
        
class ToolList:
    def __init__ (self, tooldir):
        self.tooldir = tooldir
        self.tooldirlist = os.listdir(self.tooldir)
        self.config_filename = "toolconfig.xml"
        self.tool_dict={}
        for curr_dir in self.tooldirlist:
            #print curr_dir
            if not os.path.isdir(self.tooldir + "/" + curr_dir):
                continue
            if not os.path.exists(self.tooldir+"/"+curr_dir+"/"+self.config_filename):
                continue
            config_file = open(self.tooldir+"/"+curr_dir+"/"+self.config_filename,'r')
            if config_file is None:
                print "Configuration could not be opened in " + self.tooldir+"/"+curr_dir
                continue
            doc_config = xml.dom.minidom.parse(config_file)
            try:
                toolconfigurationNode = doc_config.getElementsByTagName("toolconfiguration")[0]
            except:
                print "Configuration file in " + self.tooldir + "/" + curr_dir + \
                      " lacks \"toolconfiguration\" tag"
                continue

            # get list of "tool" nodes, continue of none found
            toolNodeList = toolconfigurationNode.getElementsByTagName("tool")
            if len(toolNodeList) == 0:
                continue

            # check for interfaces
            if len(toolNodeList[0].getElementsByTagName("interfaces"))==0:
                continue

            for toolNode in toolNodeList:
                this_tool = Tool()

                # get tool name
                if toolNode.getElementsByTagName("name")[0].hasChildNodes():
                    this_tool.name = str(toolNode.getElementsByTagName("name")[0].firstChild.data)
                else:
                    raise StandardError, "error loading tool from file " + \
                            self.tooldir + '/' + curr_dir + '/' + self.config_filename + \
                            " : tool has no name"

                # get startfile
                if toolNode.getElementsByTagName("startfile")[0].hasChildNodes():
                    this_tool.name = str(toolNode.getElementsByTagName("name")[0].firstChild.data)
                else:
                    raise StandardError, "error loading tool from file " + \
                            self.tooldir + '/' + curr_dir + '/' + self.config_filename + \
                            " : tool has no startfile"

                # get module name
                if toolNode.getElementsByTagName("modulename")[0].hasChildNodes():
                    this_tool.modulename = str(toolNode.getElementsByTagName("modulename")[0].firstChild.data)
                else:
                    this_tool.modulename = "<unknown module>"

                # get package name
                if toolNode.getElementsByTagName("packagename")[0].hasChildNodes():
                    this_tool.packagename = str(toolNode.getElementsByTagName("packagename")[0].firstChild.data)
                else:
                    this_tool.packagename = "<unknown package>"

                # get list of "interface" nodes, assuming they exist under "interfaces" node
                interfaceNodeList = toolNode.getElementsByTagName("interface")
                if len(interfaceNodeList) > 0:
                    for interfaceNode in interfaceNodeList:
                        this_interface = ToolInterface()
                        this_interface.name = str(interfaceNode.getAttribute("name"))
                        this_interface.namespace = str(interfaceNode.getAttribute("namespace"))
                        this_tool.interface.append(this_interface)
                    for i in range(0,len(this_tool.interface)):
                        if not self.tool_dict.has_key(this_tool.interface[i].namespace):
                            self.tool_dict[this_tool.interface[i].namespace]={}
                            #print "Creating interface ns: " + this_tool.interface[i].namespace
                        if not self.tool_dict[this_tool.interface[i].namespace].has_key(this_tool.interface[i].name):
                            self.tool_dict[this_tool.interface[i].namespace][this_tool.interface[i].name]=[]
                            #print "Creating interface name: " + this_tool.interface[i].name
                        self.tool_dict[this_tool.interface[i].namespace][this_tool.interface[i].name].append(this_tool)
    def getSupportedTools(self,namespace,interface):
        if not self.tool_dict.has_key(namespace):
            return None
        if not self.tool_dict[namespace].has_key(interface):
            return None
        return self.tool_dict[namespace][interface]


#----------------------------------------------------------------------
def LoadConfiguration(frame_obj):
    # Load configuration file
#    alf_cfg = "alf.cfg"
    root = __file__
    if os.path.islink (root):
        root = os.path.realpath (root)
    root = os.path.dirname (os.path.abspath (root))
    alf_cfg = root + '/config/alf.cfg'
    if not os.path.isfile(alf_cfg):
        print 'Configuration file "alf.cfg" missing from path'
        return None
    alfcfgfile = open(alf_cfg, 'r')
    doc_cfg = xml.dom.minidom.parse(alfcfgfile)
    alfconfigurationNodeList = doc_cfg.getElementsByTagName("alfconfiguration")
    if len(alfconfigurationNodeList) == 0:
        raise StandardError, "ALF configuration file does not include \"alfconfiguration\" node"

    if doc_cfg.getElementsByTagName("version")[0].hasChildNodes():
        frame_obj.version = str(doc_cfg.getElementsByTagName("version")[0].firstChild.data)
    else:
        frame_obj.version = "Version Unknown"

    if doc_cfg.getElementsByTagName("installpath")[0].hasChildNodes():
        frame_obj.installPath = str(doc_cfg.getElementsByTagName("installpath")[0].firstChild.data)
    else:
        frame_obj.installPath = ""

    if doc_cfg.getElementsByTagName("stdidlpath")[0].hasChildNodes():
        frame_obj.stdidlpath = str(doc_cfg.getElementsByTagName("stdidlpath")[0].firstChild.data)
    else:
        frame_obj.stdidlpath = ""

    if doc_cfg.getElementsByTagName("ossieincludepath")[0].hasChildNodes():
        frame_obj.ossieincludepath = str(doc_cfg.getElementsByTagName("ossieincludepath")[0].firstChild.data)
    else:
        frame_obj.ossieincludepath = ""

    if doc_cfg.getElementsByTagName("homedir")[0].hasChildNodes():
        frame_obj.homedir = str(doc_cfg.getElementsByTagName("homedir")[0].firstChild.data)
    else:
        frame_obj.homedir = "/sdr"
    
    # Find all tools in the tools directory
    # NOTE: Tools directory is now now the alf_plugins module in site_packages
    #tooldir = frame_obj.installpath+"/tools"
    # NOTE: There may be a better way of finding the alf_plugins directory, but this works
    tooldir = __import__('alf_plugins').__file__
    if os.path.islink (tooldir):
        tooldir = os.path.realpath (tooldir)
    tooldir = os.path.dirname (os.path.abspath (tooldir))
    frame_obj.tools = ToolList(tooldir)

#----------------------------------------------------------------------
def importStandardIdl(parent):   
            
    #if os.path.isfile(self.parent.ossieIncludePath + "cf.idl"):
    #    cfIdl_file = self.parent.ossieIncludePath + "cf.idl"
    if os.path.isfile("/usr/local/include/ossie/" + "cf.idl"):
        cfIdl_file = "/usr/local/include/ossie/" + "cf.idl"
    else:
        tmpstr = "Cannot find cf.idl in the OSSIE installation location:\n"
        tmpstr += parent.ossieIncludePath
        errorMsg(parent,tmpstr)

    # for each file in the standardinterfaces directory, import all available
    # interfaces (skip standardIdl files)
    stdIdlPath = "/usr/local/include/standardinterfaces/"
    idlList = os.listdir(stdIdlPath)
    if len(idlList) <= 0:
        tmpstr = "Can't find any files in: " + parent.stdIdlPath
        errorMsg(parent,tmpstr)
        return
        
    # Add the CF interfaces first - in case another file includes them, we
    # don't want them asscociated with anything other than cf.idl
    Available_Ints = []
    Available_Ints.extend(importIDL.getInterfaces(cfIdl_file))
        
    for idl_file in idlList:
        # standardIdl files are not included because they are aggregates of the other interfaces
        if 'standardIdl' in idl_file:
            continue

        if 'customInterfaces' in idl_file:
            continue
            
        if idl_file[-3:] != "idl":
            continue
            
        tempInts = importIDL.getInterfaces(stdIdlPath+idl_file)
        for t in tempInts:
            if t not in Available_Ints:
               # print "Testing: " + t.name + " " + idl_file + " " + str(len(self.Available_Ints))
                Available_Ints.append(t)
	        #if t.name == 'timingStatus':
		    #    self.timing_interface = CC.Interface(t.name, t.nameSpace, t.operations, t.filename, t.fullpath)
			#self.timing_port = CC.Port('send_timing_report', self.timing_interface, "Uses", "data")
#		    print "CF.py: " + t.name + "  " + str(len(t.operations))

    return Available_Ints

#----------------------------------------------------------------------
def GetCollapsedIconData():
    return \
'\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\x00\x00\x00\x10\x08\x06\
\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\x08\x08\x08|\x08d\x88\x00\
\x00\x01\x8eIDAT8\x8d\xa5\x93-n\xe4@\x10\x85?g\x03\n6lh)\xc4\xd2\x12\xc3\x81\
\xd6\xa2I\x90\x154\xb9\x81\x8f1G\xc8\x11\x16\x86\xcd\xa0\x99F\xb3A\x91\xa1\
\xc9J&\x96L"5lX\xcc\x0bl\xf7v\xb2\x7fZ\xa5\x98\xebU\xbdz\xf5\\\x9deW\x9f\xf8\
H\\\xbfO|{y\x9dT\x15P\x04\x01\x01UPUD\x84\xdb/7YZ\x9f\xa5\n\xce\x97aRU\x8a\
\xdc`\xacA\x00\x04P\xf0!0\xf6\x81\xa0\xf0p\xff9\xfb\x85\xe0|\x19&T)K\x8b\x18\
\xf9\xa3\xe4\xbe\xf3\x8c^#\xc9\xd5\n\xa8*\xc5?\x9a\x01\x8a\xd2b\r\x1cN\xc3\
\x14\t\xce\x97a\xb2F0Ks\xd58\xaa\xc6\xc5\xa6\xf7\xdfya\xe7\xbdR\x13M2\xf9\
\xf9qKQ\x1fi\xf6-\x00~T\xfac\x1dq#\x82,\xe5q\x05\x91D\xba@\xefj\xba1\xf0\xdc\
zzW\xcff&\xb8,\x89\xa8@Q\xd6\xaaf\xdfRm,\xee\xb1BDxr#\xae\xf5|\xddo\xd6\xe2H\
\x18\x15\x84\xa0q@]\xe54\x8d\xa3\xedf\x05M\xe3\xd8Uy\xc4\x15\x8d\xf5\xd7\x8b\
~\x82\x0fh\x0e"\xb0\xad,\xee\xb8c\xbb\x18\xe7\x8e;6\xa5\x89\x04\xde\xff\x1c\
\x16\xef\xe0p\xfa>\x19\x11\xca\x8d\x8d\xe0\x93\x1b\x01\xd8m\xf3(;x\xa5\xef=\
\xb7w\xf3\x1d$\x7f\xc1\xe0\xbd\xa7\xeb\xa0(,"Kc\x12\xc1+\xfd\xe8\tI\xee\xed)\
\xbf\xbcN\xc1{D\x04k\x05#\x12\xfd\xf2a\xde[\x81\x87\xbb\xdf\x9cr\x1a\x87\xd3\
0)\xba>\x83\xd5\xb97o\xe0\xaf\x04\xff\x13?\x00\xd2\xfb\xa9`z\xac\x80w\x00\
\x00\x00\x00IEND\xaeB`\x82' 

def GetCollapsedIconBitmap():
    return wx.BitmapFromImage(GetCollapsedIconImage())

def GetCollapsedIconImage():
    import cStringIO
    stream = cStringIO.StringIO(GetCollapsedIconData())
    return wx.ImageFromStream(stream)

#----------------------------------------------------------------------
def GetExpandedIconData():
    return \
'\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x10\x00\x00\x00\x10\x08\x06\
\x00\x00\x00\x1f\xf3\xffa\x00\x00\x00\x04sBIT\x08\x08\x08\x08|\x08d\x88\x00\
\x00\x01\x9fIDAT8\x8d\x95\x93\xa1\x8e\xdc0\x14EO\xb2\xc4\xd0\xd2\x12\xb7(mI\
\xa4%V\xd1lQT4[4-\x9a\xfe\xc1\xc2|\xc6\xc2~BY\x83:A3E\xd3\xa0*\xa4\xd2\x90H!\
\x95\x0c\r\r\x1fK\x81g\xb2\x99\x84\xb4\x0fY\xd6\xbb\xc7\xf7>=\'Iz\xc3\xbcv\
\xfbn\xb8\x9c\x15 \xe7\xf3\xc7\x0fw\xc9\xbc7\x99\x03\x0e\xfbn0\x99F+\x85R\
\x80RH\x10\x82\x08\xde\x05\x1ef\x90+\xc0\xe1\xd8\ryn\xd0Z-\\A\xb4\xd2\xf7\
\x9e\xfbwoF\xc8\x088\x1c\xbbae\xb3\xe8y&\x9a\xdf\xf5\xbd\xe7\xfem\x84\xa4\
\x97\xccYf\x16\x8d\xdb\xb2a]\xfeX\x18\xc9s\xc3\xe1\x18\xe7\x94\x12cb\xcc\xb5\
\xfa\xb1l8\xf5\x01\xe7\x84\xc7\xb2Y@\xb2\xcc0\x02\xb4\x9a\x88%\xbe\xdc\xb4\
\x9e\xb6Zs\xaa74\xadg[6\x88<\xb7]\xc6\x14\x1dL\x86\xe6\x83\xa0\x81\xba\xda\
\x10\x02x/\xd4\xd5\x06\r\x840!\x9c\x1fM\x92\xf4\x86\x9f\xbf\xfe\x0c\xd6\x9ae\
\xd6u\x8d \xf4\xf5\x165\x9b\x8f\x04\xe1\xc5\xcb\xdb$\x05\x90\xa97@\x04lQas\
\xcd*7\x14\xdb\x9aY\xcb\xb8\\\xe9E\x10|\xbc\xf2^\xb0E\x85\xc95_\x9f\n\xaa/\
\x05\x10\x81\xce\xc9\xa8\xf6><G\xd8\xed\xbbA)X\xd9\x0c\x01\x9a\xc6Q\x14\xd9h\
[\x04\xda\xd6c\xadFkE\xf0\xc2\xab\xd7\xb7\xc9\x08\x00\xf8\xf6\xbd\x1b\x8cQ\
\xd8|\xb9\x0f\xd3\x9a\x8a\xc7\x08\x00\x9f?\xdd%\xde\x07\xda\x93\xc3{\x19C\
\x8a\x9c\x03\x0b8\x17\xe8\x9d\xbf\x02.>\x13\xc0n\xff{PJ\xc5\xfdP\x11""<\xbc\
\xff\x87\xdf\xf8\xbf\xf5\x17FF\xaf\x8f\x8b\xd3\xe6K\x00\x00\x00\x00IEND\xaeB\
`\x82' 

def GetExpandedIconBitmap():
    return wx.BitmapFromImage(GetExpandedIconImage())

def GetExpandedIconImage():
    import cStringIO
    stream = cStringIO.StringIO(GetExpandedIconData())
    return wx.ImageFromStream(stream)

def getDOM(fileMgr, fileName):
        #get the file descriptor from the framework
        #file path should be absolute to the Framework file system
        #e.g. if framework is mounted on /sdr/dom, then the path of a
        #sad file would like "/waveforms/ossie_demo/ossie_demo.sad.xml
        
        #try:
            #print "getDOM() - fileName = ", fileName 
            fd = fileMgr.open(str(fileName), True)
            domObj = xml.dom.minidom.parse(fd)
            fd.close()
            return domObj
#        except:
#            print sys.exc_info()[0]
#            errorMsg = str(sys.exc_info()[1])
#            errorMsg = errorMsg + "\ngetDOM(): Could not create DOM for file '" + fileName + "'"
#            print str(errorMsg)
#            return None
#    
def getFileList(fileMgr, dir=None, filetype= ""):
    """ This function searches for the given filetype (e.g. *.sad.xml)
    under the dir through the FileManager. the directory dir is relative 
    to the FileManager mount directory (/sdr/dom)
    """
    
    fileNames = []
    try:
        fileObjList = fileMgr.list("/"+filetype) #FileMgr list func demands a /
        #fileObjList is a CORBA sequence of CF:File objects. extract the file name 
        
        if fileObjList is not None:
            for fileObj in fileObjList:
                #the file name is stored as an absolute path to the FileManager file system
                #e.g file name = dom/waveforms/ossie_demo/ossie_demo.sad.xml
                #throughout wavedash,we store the filenames in the model as relative to the
                #FileManager root dir (/sdr/dom). Hence strip off dom/ from the returned filename
                if dir is not None:
                    if ( fileObj.name.find(dir) != -1 ):
                        fName = fileObj.name[fileObj.name.find("/"):]
                        fileNames.append(fName)
    except:
        error = str(sys.exc_info()[1])
        error = error + "\n getFileList(): Could not get list of files from FileManager '"
        print str(error)
        return None
    
    return fileNames
#----------------------------------------------------------------------
