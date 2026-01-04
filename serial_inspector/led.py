import tkinter as tk

class LEDIndicator:
    def __init__(self, parent, size=50, booleanvar=None):
        """
        Initialize the LED indicator.
        
        :param parent: The parent widget (e.g., a Tkinter window or frame).
        :param size: The diameter of the LED circle.
        """
        self.size = size
        self.led_on = False  # Initial state is OFF
        
        # Create a Canvas to draw the LED
        self.canvas = tk.Canvas(parent, width=size, height=size, bg="white", highlightthickness=0)
        #self.canvas.pack(pady=10)
        
        # Draw the LED circle
        self.led_circle = self.canvas.create_oval(2, 2, size-2, size-2, fill="red", outline="black", width=2)
        self.booleanvar = booleanvar
        
        if self.booleanvar is not None:
            self.booleanvar.trace_add('write', self.on_var_changed)
            # Set initial state based on the variable
            self.set_state(self.booleanvar.get())
        
    def grid(self, **kwargs):
        self.canvas.grid(**kwargs)
       
            
    def on_var_changed(self, *args):
        if self.booleanvar is not None:
            self.set_state(self.booleanvar.get())
            
    def set_state(self, state):
        """
        Set the LED state.
        
        :param state: True to turn ON the LED, False to turn it OFF.
        """
        self.led_on = state
        color = "red" if state else "green"
        self.canvas.itemconfig(self.led_circle, fill=color)

# Example usage
if __name__ == "__main__":
    # Initialize the main window
    root = tk.Tk()
    root.title("LED Indicator Class Example")
    root.geometry("200x200")
    
    # Create an LED indicator with a size of 80 pixels
    led = LEDIndicator(root, size=80)
    
    # Function to toggle the LED state
    def toggle_led():
        led.set_state(not led.led_on)
    
    # Create a button to toggle the LED
    toggle_button = tk.Button(root, text="Toggle LED", command=toggle_led)
    toggle_button.pack(pady=10)
    
    # Run the Tkinter event loop
    root.mainloop()
