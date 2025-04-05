import re

def extract_syscall_data():
    with open('fs.img', 'rb') as f:
        content = f.read().decode('utf-8', errors='ignore')
        matches = re.findall(r'(\d+)\s+(\d+)\n', content)
        
        with open('syscall_plot.csv', 'w') as out:
            out.write('Minute,Calls\n')
            for minute, calls in matches:
                out.write(f'{minute},{calls}\n')

if __name__ == '__main__':
    extract_syscall_data()

