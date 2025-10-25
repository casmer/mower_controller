# GUI design
from tkinter import N, S, E, W
import tkinter as tk
from tkinter import scrolledtext
import led
# Communication with serial port
import serial
from serial.tools import list_ports
from enum import Enum, auto
# Multi-threading
import threading

# Get path
import os

# Use realpath if you want the real path (symlinks are resolved)
# file_path = os.path.realpath(__file__)
FILE_PATH = os.path.abspath(__file__)
ICON_PATH = os.path.join(os.path.dirname(__file__), "icon.png")

class ChannelInfo:
    def __init__(self, number, name):
        self.number = number
        self.name = name

class IOType(Enum):
    DIGITAL = 1
    ANALOG = 2

class MowerIOInfo:
    def __init__(self, number, name, io_type):
        self.number = number
        self.name = name
        self.io_type = io_type
        self.value = None
class GUI:
    # GUI main class
    def __init__(self, title):

        self.portNamesList = []
        self.baudRatesList = [
            1200,
            2400,
            4800,
            9600,
            19200,
            38400,
            57600,
            115200,
            230400,
            460800,
            576000,
            921600,
        ]
        
        self.channel_config = [
            ChannelInfo(3, "Left Stick Y"),
            ChannelInfo(4, "Left Stick X" ),
            ChannelInfo(1, "Right Stick X"),
            ChannelInfo(2, "Right Stick Y"),
            ChannelInfo(5, "Channel 5" ),
            ChannelInfo(6, "Channel 6" ),
            ChannelInfo(7, "Channel 7" ),
            ChannelInfo(8, "Channel 8" ),
            ChannelInfo(9, "Channel 9" ),
            ChannelInfo(10, "Channel 10" ),
            ChannelInfo(11, "Channel 11" ),
            ChannelInfo(12, "Channel 12" ),
            ChannelInfo(13, "Channel 13" ),
            ChannelInfo(14, "Channel 14" ),
            ChannelInfo(15, "Channel 15" ),
            ChannelInfo(16, "Channel 16" ),
        ]
        
        self.mower_dio_input_config = [        
            MowerIOInfo(1 ,"left throttle_position", IOType.ANALOG),
            MowerIOInfo(2 ,"left zero", IOType.DIGITAL),
            MowerIOInfo(3 ,"left zero pos", IOType.ANALOG),
            MowerIOInfo(4 ,"right throttle pos", IOType.ANALOG),
            MowerIOInfo(5 ,"right zero", IOType.DIGITAL),
            MowerIOInfo(6 ,"right zero pos", IOType.ANALOG),
            MowerIOInfo(7 ,"seat drive", IOType.DIGITAL),
            MowerIOInfo(8 ,"seat blade", IOType.DIGITAL),
            MowerIOInfo(9 ,"low speed drive", IOType.DIGITAL),
            MowerIOInfo(10 ,"brake engaged", IOType.DIGITAL),
            MowerIOInfo(11 ,"low speed cut", IOType.DIGITAL),
            MowerIOInfo(12 ,"blades enabled", IOType.DIGITAL),
        ]
        
        self.mower_dio_output_config = [        
            MowerIOInfo(1 ,"left throttle_position", IOType.ANALOG),
            MowerIOInfo(2 ,"left zero", IOType.DIGITAL),
            MowerIOInfo(3 ,"left zero pos", IOType.ANALOG),
            MowerIOInfo(4 ,"right throttle pos", IOType.ANALOG),
            MowerIOInfo(5 ,"right zero", IOType.DIGITAL),
            MowerIOInfo(6 ,"right zero pos", IOType.ANALOG),
            MowerIOInfo(7 ,"seat drive", IOType.DIGITAL),
            MowerIOInfo(8 ,"seat blade", IOType.DIGITAL),
            MowerIOInfo(9 ,"low speed drive", IOType.DIGITAL),
            MowerIOInfo(10 ,"brake engaged", IOType.DIGITAL),
            MowerIOInfo(11 ,"low speed cut", IOType.DIGITAL),
            MowerIOInfo(12 ,"blades enabled", IOType.DIGITAL),
        ]
        
        self.isAnyPortAvailable = False
        self.isStarted = False
        self.serialPortName = None
        self.serialPortBaud = 115200
        self.partial_line = b''
        self.serialPortManager = SerialPortManager(self.serialPortBaud)
        self.get_available_serial_ports()

        self.guiUpdateInterval = 40

        self.window = tk.Tk()
        # Title of application window
        self.window.title(title)
        # Icon of application window
        self.window.iconphoto(False, tk.PhotoImage(file=ICON_PATH))

        self.topFrame = tk.Frame(self.window, bg="#cccccc")

        self.topFrame
        self.scanButton = tk.Button(
            self.topFrame,
            text="Scan Serial Ports",
            bg="#0051ff",
            fg="#ffffff",
            border=0,
            highlightbackground="#ffffff",
            highlightthickness=2,
            activebackground="#1f7cff",
            activeforeground="#ffffff",
            font=("Sans", "10", "bold"),
            command=self.scan_button_command,
        )       

        # Define a tk.StringVar for storing selected item in OptionMenu
        self.selectedPort = tk.StringVar()
        # Set default value of selectedPort
        if not self.isAnyPortAvailable:
            self.portNamesList = ["No ports available"]
        self.selectedPort.set(self.portNamesList[0])

        self.portsOptionMenu = tk.OptionMenu(
            self.topFrame, self.selectedPort, *self.portNamesList
        )

        self.portsOptionMenu.configure(
            bg="#ffffff",
            fg="#222222",
            border=0,
            highlightbackground="#aaaaaa",
            activebackground="#eeeeee",
            activeforeground="#111111",
            direction="left",
            font=("Sans", "10", "bold"),
        )
        if self.isAnyPortAvailable == False:
            self.portsOptionMenu.configure(state="disabled")

        # Define a tk.IntVar for storing selected item in OptionMenu
        self.selectedBaudRate = tk.IntVar()
        # Set default value of selectedBaudRate
        self.selectedBaudRate.set(self.baudRatesList[7])
        self.baudRatesOptionMenu = tk.OptionMenu(
            self.topFrame, self.selectedBaudRate, *self.baudRatesList
        )

        self.baudRatesOptionMenu.configure(
            bg="#ffffff",
            fg="#222222",
            border=0,
            highlightbackground="#aaaaaa",
            activebackground="#eeeeee",
            activeforeground="#111111",
            direction="left",
            font=("Sans", "10", "bold"),
        )
        if self.isAnyPortAvailable == False:
            self.baudRatesOptionMenu.configure(state="disabled")

        self.connectButton = tk.Button(
            self.topFrame,
            text="Connect",
            bg="#00a832",
            fg="#ffffff",
            border=0,
            highlightbackground="#ffffff",
            highlightthickness=2,
            activebackground="#3fcc69",
            activeforeground="#ffffff",
            font=("Sans", "10", "bold"),
            command=self.start_button_command,
        )
        if self.isAnyPortAvailable == False:
            self.connectButton.configure(
                state="disabled", bg="#bbbbbb", highlightbackground="#aaaaaa"
            )

        self.textBox = scrolledtext.ScrolledText(
            self.topFrame,
            bg="#222222",
            fg="#eeeeee",
            border=0,
            wrap="none",
            highlightbackground="#aaaaaa",
            highlightthickness=2,
            font=("Sans", "10", "bold"),
        )

        # Start updating textbox in GUI
        self.recursive_update_textbox()

        ###############################
        ## Widgets size and position ##
        ###############################

        spacing = 10
        padding = 10
        widget_width = 400
        window_width = widget_width*2 + 2 * padding
        window_height = 1200

        # Size of application window
        self.window.geometry("{}x{}".format(window_width, window_height))
        # Don't allow resizing in the x or y direction
        #self.window.resizable(False, False)
        
        # tracks the current row
        current_row = 0
        self.topFrame.configure(padx=padding, pady=padding)
        self.topFrame.grid(column=0, row=current_row, sticky=(N, S, E, W))
        current_row += 1
        self.scanButton.configure( padx=padding, pady=padding)
        self.scanButton.grid(column=0, row=current_row, columnspan=3, sticky=(W, N))

        self.portsOptionMenu.configure( padx=padding, pady=padding)
        self.portsOptionMenu.grid(column=3, row=current_row, columnspan=2, sticky=(N, W, E, S))

        self.baudRatesOptionMenu.configure( padx=padding, pady=padding)
        self.baudRatesOptionMenu.grid(column=5, row=current_row,columnspan=2, sticky=(N, E, W, S))

        self.connectButton.configure( padx=padding, pady=padding)
        self.connectButton.grid(column=7, row=current_row, sticky=(N, E))

        current_row += 1
        self.inputs_label = tk.Label(self.topFrame, text="RC Controller Inputs")
        self.inputs_label.grid(column=0, row=current_row, columnspan=8, sticky=(N, E,W,S))
        current_row += 1
        
        self.radio_channels = []
        self.radio_channel_labels = []
        self.radio_channel_values = []
        for position in range(0,16):
            self.radio_channel_values.append(tk.IntVar())
        column = 0
        row = current_row
        for position in range(0,16):
            channelInfo = self.channel_config[position]
            row = current_row + position // 4
            self.radio_channel_labels.append(tk.Label(self.topFrame, text=channelInfo.name))
            self.radio_channels.append(tk.Label(self.topFrame, textvariable=self.radio_channel_values[channelInfo.number - 1]))
            
            self.radio_channel_labels[-1].grid(column=column, row=row, sticky=(N, E,W,S))
            column +=1
            self.radio_channels[-1].grid(column=column, row=row, sticky=(N, E,W,S))
            column += 1
            if column >= 8:
                column = 0

        current_row = row + 1   
        self.inputs_label = tk.Label(self.topFrame, text="Mower DIO Inputs")
        self.inputs_label.grid(column=0, row=current_row, columnspan=8, sticky=(N, E,W,S))
        current_row += 1
        
        self.mower_input_io = []
        self.mower_input_io_labels = []
        column = 0
        row = current_row
        for ioInfo in self.mower_dio_input_config:
            if ioInfo.io_type == IOType.DIGITAL:
                ioInfo.value = tk.BooleanVar()
            else:
                ioInfo.value = tk.IntVar()
        
        for position in range(0,12):
            ioInfo = self.mower_dio_input_config[position]
            row = current_row + position // 4
            self.mower_input_io_labels.append(tk.Label(self.topFrame, text=ioInfo.name))
            self.mower_input_io.append(tk.Label(self.topFrame, textvariable=ioInfo.value))
            
            self.mower_input_io_labels[-1].grid(column=column, row=row, sticky=(N, E,W,S))
            column +=1
            self.mower_input_io[-1].grid(column=column, row=row, sticky=(N, E,W,S))
            column += 1
            if column >= 8:
                column = 0
                
        current_row = row + 1  
        
        self.outputs_label = tk.Label(self.topFrame, text="Mower DIO Outputs")
        self.outputs_label.grid(column=0, row=current_row, columnspan=8, sticky=(N, E,W,S))
        current_row += 1
        
        self.mower_output_io = []
        self.mower_output_io_labels = []
        column = 0
        row = current_row
        for ioInfo in self.mower_dio_output_config:
            if ioInfo.io_type == IOType.DIGITAL:
                ioInfo.value = tk.BooleanVar()
            else:
                ioInfo.value = tk.IntVar()
        
        for position in range(0,12):
            ioInfo = self.mower_dio_output_config[position]
            row = current_row + position // 4
            self.mower_output_io_labels.append(tk.Label(self.topFrame, text=ioInfo.name))
            self.mower_output_io.append(tk.Label(self.topFrame, textvariable=ioInfo.value))
            
            self.mower_output_io_labels[-1].grid(column=column, row=row, sticky=(N, E,W,S))
            column +=1
            self.mower_output_io[-1].grid(column=column, row=row, sticky=(N, E,W,S))
            column += 1
            if column >= 8:
                column = 0

        current_row = row + 1   
      
        
        
        self.textBox.configure( padx=padding, pady=padding)
        self.textBox.grid(column=0, row=current_row, columnspan=8, sticky=(N, E,W,S))

        self.window.columnconfigure(0, weight=1)
        self.window.rowconfigure(0, weight=1)
        
        for col_num in range(1, 8):
            self.topFrame.columnconfigure(col_num, weight=(1+col_num//2))

        for child in self.topFrame.winfo_children(): 
            child.grid_configure(padx=5, pady=5)

        self.window.protocol("WM_DELETE_WINDOW", self.close_window)
        self.scan_button_command()
        # Blocking loop for GUI (Always put at the end)
        self.window.mainloop()

    def start_button_command(self):

        if not self.isStarted:
            self.isStarted = True
            self.connectButton.configure(
                bg="#ba0020",
                highlightbackground="#ffffff",
                activebackground="#cf324d",
                text="Disconnect",
            )
            # Get desired serial port name
            self.serialPortName = self.selectedPort.get()
            # Get desired serial port baud rate
            self.serialPortBaud = self.selectedBaudRate.get()
            # Start Serial Port Communication
            self.serialPortManager.set_name(self.serialPortName)
            self.serialPortManager.set_baud(self.serialPortBaud)
            self.serialPortManager.start()
            # Start updating textbox in GUI
            self.recursive_update_textbox()

        else:
            self.isStarted = False
            self.connectButton.configure(
                bg="#00a832",
                highlightbackground="#ffffff",
                activebackground="#3fcc69",
                text="Connect",
            )
            self.serialPortManager.stop()

    def scan_button_command(self):
        self.portNamesList = self.get_available_serial_ports()

        if len(self.portNamesList) == 0:
            self.isAnyPortAvailable = False
            self.portNamesList = ["No ports available"]
            self.portsOptionMenu.configure(state="disabled")
            self.baudRatesOptionMenu.configure(state="disabled")
            self.connectButton.configure(
                state="disabled", bg="#bbbbbb", highlightbackground="#aaaaaa"
            )
        else:
            self.isAnyPortAvailable = True
            self.portsOptionMenu.configure(state="normal")
            self.baudRatesOptionMenu.configure(state="normal")
            
            if self.isStarted:
                self.connectButton.configure(
                    bg="#ba0020",
                    highlightbackground="#ffffff",
                    activebackground="#cf324d",
                    state="normal",
                )
            else:
                self.connectButton.configure(
                    bg="#00a832",
                    highlightbackground="#ffffff",
                    activebackground="#3fcc69",
                    state="normal",
                )

        self.update_option_menu(self.portNamesList)

    def get_available_serial_ports(self):
        # Clear portNames list
        portNames = []
        # Get a list of available serial ports
        portsList = list_ports.comports()
        # Sort based on port names
        portsList = sorted(portsList)

        for port in portsList:
            portNames.append(port.device)

        return portNames

    def update_option_menu(self, portNames):
        # Remove old items
        self.portsOptionMenu["menu"].delete(0, "end")
        # Add new items
        for portName in portNames:
            self.portsOptionMenu["menu"].add_command(
                label=portName, command=tk._setit(self.selectedPort, portName)
            )
        # Set default value of selectedPort
        self.selectedPort.set(portNames[0])

    def process_incoming_sbus_line(self, line):
        channel_values = line[6:].strip().split(b'\t')
        for i in range(min(16, len(channel_values))):
            try:
                self.radio_channel_values[i].set(int(channel_values[i]))
            except ValueError:
                print("sbus ValueError on channel", i, "with value", channel_values[i])
                self.radio_channel_values[i].set(-1)
                
    def process_incoming_io_input_line(self, line):
        io_input_values = line[6:-7].strip().split(b'\t')
        for ioInfo in self.mower_dio_input_config:
            try:
                if (ioInfo.io_type == IOType.DIGITAL):
                    ioInfo.value.set(bool(int(io_input_values[ioInfo.number - 1])))
                else:
                    ioInfo.value.set(int(io_input_values[ioInfo.number - 1]))
                
            except ValueError:
                print("input ValueError on channel", ioInfo.number, " value: ", io_input_values[ioInfo.number - 1], " processing line:", io_input_values)
                ioInfo.value.set(0)
                continue
            except IndexError:
                print("input IndexError on channel", ioInfo.number, "processing line:", io_input_values)
                ioInfo.value.set(0)
                continue
    def process_incoming_io_output_line(self, line):
        io_output_values = line[7:-8].strip().split(b'\t')
        for ioInfo in self.mower_dio_output_config:
            try:
                if (ioInfo.io_type == IOType.DIGITAL):
                    ioInfo.value.set(bool(int(io_output_values[ioInfo.number - 1])))
                else:
                    ioInfo.value.set(int(io_output_values[ioInfo.number - 1]))
                
            except ValueError:
                print("output ValueError on channel", ioInfo.number,  " value: ", io_output_values[ioInfo.number - 1], " processing line:", io_output_values)
                ioInfo.value.set(0)
                continue
            except IndexError:
                print("output IndexError on channel", ioInfo.number, "processing line:", io_output_values)
                ioInfo.value.set(0)
                continue
            
    def recursive_update_textbox(self):
        serialPortBuffer = self.serialPortManager.read_buffer()
        # Update textbox in a kind of recursive function using Tkinter after() method
        
        if len(serialPortBuffer) > 0:
            lines = serialPortBuffer.split(b'\n')
            for line in lines:
                if not line.endswith(b'\r'):
                    self.partial_line += line
                else:
                    if len(self.partial_line) > 0:
                        line = self.partial_line + line
                        self.partial_line = b''
                    if line.startswith(b'sbsus:'):
                        if line.endswith(b':sb\r'):
                            self.process_incoming_sbus_line(line)
                        else:
                            self.sbus_partial_line = line
                    elif line.startswith(b'input:'):
                        if line.endswith(b':input\r'):
                            self.process_incoming_io_input_line(line)
                        else:
                            self.sbus_partial_line = line
                    elif line.startswith(b'output:'):
                        if line.endswith(b':output\r'):
                            self.process_incoming_io_output_line(line)
                        else:
                            self.sbus_partial_line = line
                    else:
                        self.textBox.insert(tk.INSERT, line.decode("ascii", errors='ignore') + '\n')
        
        # autoscroll to the bottom
        self.textBox.see(tk.END)
        # Recursively call recursive_update_textbox using Tkinter after() method
        if self.serialPortManager.isRunning:
            self.window.after(self.guiUpdateInterval, self.recursive_update_textbox)

    def close_window(self):
        if self.isStarted:
            self.serialPortManager.stop()
        self.window.destroy()


class SerialPortManager:
    # A class for management of serial port data in a separate thread
    def __init__(self, serialPortBaud=9600):
        self.isRunning = False
        self.serialPortName = None
        self.serialPortBaud = serialPortBaud
        self.serialPort = serial.Serial()
        # Create a byte array to store incoming data
        self.serialPortBuffer = bytearray()

    def set_name(self, serialPortName):
        self.serialPortName = serialPortName

    def set_baud(self, serialPortBaud):
        self.serialPortBaud = serialPortBaud

    def start(self):
        self.isRunning = True
        self.serialPortThread = threading.Thread(target=self.thread_handler)
        self.serialPortThread.start()

    def stop(self):
        self.isRunning = False

    def thread_handler(self):

        while self.isRunning:

            if not self.serialPort.isOpen():

                self.serialPort = serial.Serial(
                    port=self.serialPortName,
                    baudrate=self.serialPortBaud,
                    bytesize=8,
                    timeout=2,
                    stopbits=serial.STOPBITS_ONE,
                )
            else:
                # Wait until there is data waiting in the serial buffer
                while self.serialPort.in_waiting > 0:
                    # Read only one byte from serial port
                    serialPortByte = self.serialPort.read(1)
                    self.serialPortBuffer.append(int.from_bytes(serialPortByte, byteorder='big'))

        if self.serialPort.isOpen():
            self.serialPort.close()

    def read_buffer(self):
        # Return a copy of serial port buffer
        buffer = self.serialPortBuffer
        # Clear serial port buffer
        self.serialPortBuffer = bytearray()
        return buffer

    def __del__(self):
        if self.serialPort.isOpen():
            self.serialPort.close()



if __name__ == "__main__":

    # Create the GUI
    gui = GUI("Serial Port + Tkinter GUI")
