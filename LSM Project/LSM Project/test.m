close all
clear all
clc

internal = 0;
resistance = 3.3;
timeconstant = 10;

inputmin = 0.5;
inputmax = 1;

tmax = 10000;

internalValue = zeros(1,tmax);
output = zeros(1,tmax);
inputs = zeros(1,tmax);

for t = 1:25
    
    random = (inputmax-inputmin).*rand(1) + inputmin;
    internal = internal + (resistance*random - internal) / timeconstant;
    
    if internal >= 1
        
        internal = 0;
        output(t) = 1;
        
    end
    
    internalValue(t) = internal;
    
    inputs(t) = resistance*random / timeconstant;
    
    plot(internalValue, 'b')
    
    hold on
    plot(output, 'r')
    plot(inputs, 'g')
    
    ylim([0, 1])
    xlim([0, t])
    
    pause(0.01)
    
    hold off
   
end

legend('Internal','Output','Input')
title(['Spiking Neuron', newline, 'Resistance of 2.2, timeconstant of 10'])
ylabel('Value')
xlabel('timestep')









