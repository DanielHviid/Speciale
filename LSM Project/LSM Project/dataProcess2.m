close all
clear all
clc

append = 'test';

filename = 'firstversion.gif';

variables = load(['variables' append '.txt']);
depth = variables(1);
width = variables(2);
timeConstant = variables(3);
resistance = variables(4);
restingPotential = variables(5);

internalData = load(['internal' append '.txt']);

activityData = load(['activity' append '.txt']);

[N, I] = size(internalData);

N = N/depth; %% maybe width?

internal = zeros(I,I,N);
activity = zeros(I,I,N);

for n=1:N

   	internal(1:I,1:I,n) = internalData((n-1)*I+1:n*I,1:I);
    activity(1:I,1:I,n) = activityData((n-1)*I+1:n*I,1:I);
    
end

allSynapses = load(['allSynapses', append, '.txt']);

activatedSynapses = load(['activatedSynapses' append '.txt']);
progress = 1;

length = 100;
lowColor = [0, 0, 0];
highColor = [0, 0, 200]/255;
colors_p = [linspace(lowColor(1),highColor(1),length)', linspace(lowColor(2),highColor(2),length)', linspace(lowColor(3),highColor(3),length)'];

img = figure();

plotAllSynapses(img, allSynapses);
h1 = plotNeurons(img, depth, width, colors_p);

xlim([-0.5, depth - 0.5]);
ylim([-0.5, width - 0.5]);

for i = 1:N/10
   
    if exist('h2', 'var')
       delete(h2);
    end 
    
    changeNeuronPlot(img, h1, internal(:,:,i)', activity(:,:,i)', colors_p);
    [progress, h2] = plotActiveSynapses(img, activatedSynapses, progress, colors_p);
    
    saveToGif(img, i, filename)
end


function h = plotNeurons(img, depth, width, colors)

    figure(img)
    n = 0;

    h = zeros(1, depth*width);
    hold on

    for x = 0:(depth-1)
        for y = 0:(width-1)
            n = n+1;
            h(n) = plot(x,y, 'o', 'Color', colors(1,:), 'Linewidth', 1);

        end
    end
   
end

function changeNeuronPlot(img, handles, internal, activity, colors)

    figure(img)
    
    highest = max(max(internal));
    
    sizes = internal(:)*10 + 1;
    activs = activity(:)*99 + 1;
    
    for i = 1:length(handles)
       
        set(handles(i), 'LineWidth', sizes(i), 'Color', colors(activs(i),:))
        
    end
end


function plotAllSynapses(img, synapses)

    figure(img)
    
    [N m] = size(synapses);
    hold on
    for n = 2:N
        plot([synapses(n,1) synapses(n,3)], [synapses(n,2) synapses(n,4)], 'k')
    end

end

function [endPoint, h] = plotActiveSynapses(img, synapses, start, colors)

    figure(img)
    
    highest = max(synapses(:,5)); %% expend to include negative values
    
    [N m] = size(synapses);
    
    hold on
    
    h = [];
    index = 0;
    
    for n = start:N
        index = index + 1;
        if synapses(n,1) == -1
            endPoint = n + 1;
            return
        end
        for i = 1:10
        
            h(index,i) = plot([synapses(n,1) synapses(n,3)], [synapses(n,2) synapses(n,4)], 'Color', colors(i*10,:), 'Linewidth', 1.5);
    
        end
    end
    
    hold off
    endPoint = N;
    
end



function saveToGif(h, n, filename)

    drawnow 
    % Capture the plot as an image 
    frame = getframe(h); 
    im = frame2im(frame); 
    [imind,cm] = rgb2ind(im,256); 
    % Write to the GIF File 
    if n == 1 
      imwrite(imind,cm,filename,'gif', 'Loopcount',inf); 
    else 
      imwrite(imind,cm,filename,'gif','WriteMode','append'); 
    end 



end








