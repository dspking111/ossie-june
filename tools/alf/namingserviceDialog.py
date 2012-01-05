## Copyright 2005, 2006, 2007, 2008, 2009 Virginia Polytechnic Institute and State University
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

import wx

class NamingserviceDialog(wx.Dialog):
    def __init__(self, parent):
        wx.Dialog.__init__(self,
                parent=parent,
                id=-1,
                title='Target Naming Service Selection',
                pos=wx.DefaultPosition,
                size=wx.Size(310, 170))

        panel = wx.Panel(self, -1)

        vbox = wx.BoxSizer(wx.VERTICAL)
        hbox = wx.BoxSizer(wx.HORIZONTAL)

        st = wx.StaticBox(panel, -1, 'Hosts', wx.Point(5,5), wx.Size(300, 95))

        self.rbLocalhost = wx.RadioButton(panel, -1, 'Localhost (127.0.0.1)', wx.Point(15,30), wx.DefaultSize, wx.RB_GROUP)
        self.rbNethost = wx.RadioButton(panel, -1, 'Network Host:', wx.Point(15,55))
        self.tcNethost = wx.TextCtrl(panel, -1, '', wx.Point(130, 55), size=wx.Size(120, -1))
        self.tcNethost.SetMaxLength(15)
        self.tcNethost.SetValue(self.GetParent().namingservice[1])
        self.tcNethost.Bind(wx.EVT_TEXT, self.checkNetHost)

        
        if(self.GetParent().namingservice[0] == '127.0.0.1') :
            self.rbLocalhost.SetValue(True)
        else:
            self.rbNethost.SetValue(True)

        

        applyButton = wx.Button(self, -1, 'Apply', wx.DefaultPosition, wx.Size(70,30))
        closeButton = wx.Button(self, -1, 'Cancel', wx.DefaultPosition, wx.Size(70,30))
       # exitButton = wx.Button(self, -1, 'Exit', wx.DefaultPosition, wx.Size(70,30))
        applyButton.Bind(wx.EVT_BUTTON, self.OnApply)
        closeButton.Bind(wx.EVT_BUTTON, self.OnClose)
       # exitButton.Bind(wx.EVT_BUTTON, self.OnExit)

        hbox.Add(applyButton, 1)
        hbox.Add(closeButton, 1, wx.LEFT, 5)
       # hbox.Add(exitButton, 1, wx.LEFT, 5)

        vbox.Add(panel, 1)
        vbox.Add(hbox, 0, wx.ALIGN_CENTER | wx.TOP | wx.BOTTOM, 10)

        self.SetSizer(vbox)

        self.Center()
        self.ShowModal()
        
        self.Destroy()

    def OnApply(self, event):
        parent = self.GetParent()
        reload = False

        if(self.rbLocalhost.GetValue()):
            parent.namingservice[0] = '127.0.0.1'
        elif(self.rbNethost.GetValue()):
            parent.namingservice[0] = self.tcNethost.GetValue().encode('ascii', 'ignore')
            parent.namingservice[1] = parent.namingservice[0]

            
        if parent.namingservice[0] not in parent.connections:
            parent.connections[parent.namingservice[0]] = []
        
        self.EndModal(1)
        parent.init_CORBA()
        parent.refreshDisplay(True)
        parent.DisplayInstalledWaveforms()
        parent.DisplayAvailableWaveforms()
        parent.DisplayAvailableComponents()
        
        #disable compform feature when naming service is running remotely
        if (parent.namingservice[0] == '127.0.0.1'):
            parent.componentsBox.Enable(True)
        else:
            wx.MessageBox("Warning: Comp Form feature will be disabled when naming service is running remotely")
            parent.componentsBox.Enable(False)
            
        if parent.connect_frame:
            parent.connect_frame.refreshDisplay()

    def OnClose(self, event):
        self.EndModal(1)

    def OnExit(self, event):
        self.EndModal(1)
        self.GetParent().Destroy()
        
    def checkNetHost(self, event):
        self.rbNethost.SetValue(True);
        self.rbLocalhost.SetValue(False);
