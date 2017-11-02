delete(instrfindall); %prevent "cant use com port"

clear all;
Ts = 1; %Sample time initialize
t = 1:Ts:6; % support of signal

p = serial('COM8', 'BaudRate', 9600, 'DATABITS', 8); %settings for current COM port (must change com port accordingly)
set(p, 'Parity', 'None');
set(p, 'Terminator', 'LF');
fopen(p); %Opens COM port
fprintf(p, 'A'); %Write text to instrument or sends A to UART RX

pause(1); %delay for a second
fprintf(p, 'A'); %sends A to UART RX

pause(1);
fprintf(p, 'A'); %sends A to UART RX

veri=fscanf(p,'%u'); %gets UART sensor information

figure
xlabel('Time')
ylabel('Sensor Change')
title('Sensor Change over Time')
grid on;
hold on;

for t=1:Ts:6
    if (mode(t,2)==1)
        plot(t, uint8(veri(t)),'o'); %plots circles
    end
    xlim ([0 6]) %x limit
    ylim ([0 200]) %y limit
    drawnow
end

fclose(p); %closes serial connection on device
delete(p); %delete serial value
