import google.generativeai as genai
import serial 
import time

genai.configure(api_key='AIzaSyAZZiQ44lYRYoe2iF7AqO_MTlTL-r0CxOQ')

model = genai.GenerativeModel('gemini-pro')



def check_heart_rate(file_path):
    try:
        with open(file_path, 'r') as file:
            for line in file:
                # Extract the heart rate value from the line
                if "Heart Rate:" in line:
                    heart_rate_str = line.split("Heart Rate:")[1].strip()
                    heart_rate = int(heart_rate_str)
                    response = model.generate_content('Is a heart rate of' + heart_rate_str +'high for someone with underlying health conditions? (Answer only Yes or No)')
                    response.resolve()
                    if response.text == 'Yes':
                                                
                        SerialObj = serial.Serial('COM24') # COMxx  format on Windows
                                        # ttyUSBx format on Linux
                        SerialObj.baudrate = 9600  # set Baud rate to 9600
                        SerialObj.bytesize = 8   # Number of data bits = 8
                        SerialObj.parity  ='N'   # No parity
                        SerialObj.stopbits = 1   # Number of Stop bits = 1
                        time.sleep(3)
                        SerialObj.write(b'A')    #transmit 'A' (8bit) to micro/Arduino
                        SerialObj.close()          
        
                    # Check if the heart rate reaches or exceeds 100
                    # if heart_rate >= 100:
                    #     print(f"Alert: Heart rate reached {heart_rate}")
    except FileNotFoundError:
        print(f"File not found: {file_path}")
    except Exception as e:
        print(f"An error occurred: {e}")

# Example usage
file_path = 'heart_rate_log.txt'
check_heart_rate(file_path)