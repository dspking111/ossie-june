#!/usr/bin/env/python

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

'''
This module contains signal sources that can be called by AWG.py.  

To add a custom source, you must make changes in two places:

1.  Add the name that you want to appear in the AWG graphical user 
    interface to the self.available_sources dictionary as dictionary keys.  
    The corresponding values in the dictionay indicate which function is 
    called when each source is selected.

2.  Add your function that generates your custom signal anywhere below the 
    comment line "#source functions are defined below:".  Your function 
    should fit the same general format as the existing functions:

    def my_function(self):
        #define your signal here
        return your_signal
'''

import math
import random

class sources:
    def __init__(self,parent):
        self.parent = parent
        self.available_sources = {'file': 'read_file()', 
                                  'sine': 'gen_sine()', 
                                  'cosine': 'gen_cosine()',
                                  'random': 'gen_random_data()',
                                  'zeros': 'gen_zeros()',
                                  'ones': 'gen_ones()',
                                  'FM': 'gen_fm()'}


    def get_sources_list(self):
        return self.available_sources


    def gen_signal(self):
        eval_string = "self." + self.available_sources[self.parent.signal_type]
        return eval(eval_string)  # return the signal generated by 
                                  # the corresponding method


#------------------------------------------------------------------------------
# source functions are defined below:

    def read_file(self):
        '''Reads data from a file and returns the items in the file as a list.
           Parent class must have a variable "file_name" and a variable 
           "delimiter".''' 
        try:
            my_file = open(self.parent.file_name, 'r')
        except IOError:
            print "no file named " + self.parent.file_name
            return []  # return an empty packet

        data_from_file = my_file.read()   # read the file as a big string
        my_file.close()   # done reading the file.  
                          # process the string from now on

        data_from_file.strip('\n')  # \n's will not be tolerated
        data_from_file.strip('[')   # ['s will not be tolerated
        data_from_file.strip(']')   # ]'s will not be tolerated

        # break the string into a list
        data_from_file = data_from_file.split(self.parent.delimiter)  

        return data_from_file


    def gen_sine(self):
        '''Generates a sinusoid and returns it as a list.  Parent must have 
           a variable "signal_len" that corresponds to the number of elements 
           in the list.  Parent must have a variable "frequency" that 
           corresponds to frequency in cycles per packet.'''
        count = 0
        sine = []
        while count < self.parent.signal_len:
            sine.append(math.sin(self.parent.frequency*2*math.pi*count/self.parent.signal_len))
            count = count + 1

        return sine


    def gen_cosine(self):
        '''Generates a cosine and returns it as a list.  Parent must have 
           a variable "signal_len" that corresponds to the number of elements 
           in the list.  Parent must have a variable "frequency" 
           that corresponds to frequency in cycles per packet.'''

        count = 0
        cosine = []
        while count < self.parent.signal_len:
            cosine.append(math.cos(self.parent.frequency*2*math.pi*count/self.parent.signal_len))
            count = count + 1

        return cosine


    def gen_random_data(self):
        '''the rand_type variable defines which function call to use 
           in the random module'''
        data = []

        # lower() makes the comparison case insensitive
        tmp_rand_type = self.parent.rand_type.lower()  
        if tmp_rand_type == 'random':
            count = 0
            while count < self.parent.signal_len:
                data.append(random.random())
                count = count + 1
        elif tmp_rand_type == 'uniform':
            count = 0
            while count < self.parent.signal_len:
                data.append(random.uniform())
                count = count + 1
        else:
            print "random methods other than random and uniform are not supported in sources.random_data"

        return data
		

    def gen_zeros(self):
        '''returns a list of zeros'''
        data = []
        for n in range(self.parent.signal_len):
            data.append(0)
        return data

    def gen_ones(self):
        '''returns a list of ones'''
        data = []
        for n in range(self.parent.signal_len):
            data.append(1)
        return data


    def gen_fm(self):
        ''' Returns an FM tone.  Frequencies currently hard-coded. '''
        fm_data = []
        audio_tone = []
        audio_tone_freq = .1  # need to calibrate
        carrier_freq = 0      # assuming upconverting using USRP or something

        count = 0


        print "WARNING: FM not supported in signals.py yet"

        # TODO:  Since I have multiple recursions, it might be 
        # worth it to speed things up by declaring the size of the list

        while count < self.parent.signal_len:
            audio_tone.append(math.sin(audio_tone_freq*2*math.pi*count/self.parent.signal_len))
            count = count + 1

        # TODO: This loop will need some calibration
        count = 0
        while count < self.parent.signal_len:
            # use the audio tone as the input to an NCO
            fm_data.append(math.sin(audio_tone[count]))
            count += 1

        return fm_data




