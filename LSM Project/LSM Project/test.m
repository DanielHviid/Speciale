close all
clear all
clc

internal = 0;
resistance = 2.2;
timeconstant = 10;

inputmin = 0;
inputmax = 1;

tmax = 25;

internalValue = zeros(1,tmax);
output = zeros(1,tmax);
inputs = zeros(1,tmax);

for t = 1:tmax
    
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
    
    %pause(0.01)
    
    hold off
   
end

legend('Internal','Output','Input * resistance')
title(['Spiking Neuron', newline, 'Resistance of 2.2, timeconstant of 10'])
ylabel('Value')
xlabel('timestep')
% 
% %figure()
% 
% avgLength = 25;
% 
% avgOutputTemp = [zeros(1,avgLength - 1), output];
% avgOutput = output;
% 
% for t = 1:tmax
%     avgOutput(t) = 2*mean(avgOutputTemp(t:t+avgLength - 1));
% end
% 
% avgInternalValueTemp = [zeros(1,25), internalValue/5 + output];
% avginternalValue = internalValue;
% 
% for t = 1:tmax
%     avginternalValue(t) = 2*mean(avgInternalValueTemp(t:t+25));
% end
% 
% hold on
% plot(avgOutput, 'k', 'LineWidth', 5)
% plot(avginternalValue, 'b', 'LineWidth', 5)
% 
% ylim([0, max(max(max(avgOutput), max(avginternalValue)), 1)])
% 










