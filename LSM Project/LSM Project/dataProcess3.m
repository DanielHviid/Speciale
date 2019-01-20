close all
clear all
clc

append = 'test';

filename = 'secondversion.gif';

variables = load(['variables' append '.txt']);
depth = variables(1);
width = variables(2);
height = variables(3);
timeConstant = variables(3);
resistance = variables(4);
restingPotential = variables(5);

internalData = load(['internal' append '.txt']);

activityData = load(['activity' append '.txt']);

[N, I] = size(internalData);
N = N / width;
N = N / height;

internal = zeros(width, depth, height, N);
activity = zeros(width, depth, height, N);

for n=1:N
    
    for m = 1:height

        first = (n-1)*width*height+(m-1)*width + 1;
        last = first + width - 1;
        
        internal(:,:,m,n) = internalData(first:last,:);
        activity(:,:,m,n) = activityData(first:last,:);
    
    end
    
end

allSynapses = load(['allSynapses', append, '.txt']);

activatedSynapses = load(['activatedSynapses' append '.txt']);
progress = 1;

img = figure();

xlim([0 depth + 1])
ylim([0 width + 1])
zlim([0 height + 1])

neuronHandles = plotNeurons(depth, width, height);
plotAllSynapses(img, allSynapses)
view([-8, 5.2])

for n=1:N
   
    changeNeuronPlot(img, neuronHandles, internal(:,:,:,n), activity(:,:,:,n), restingPotential);
    saveToGif(img, n, filename)
    
end

title('100th timestep 10 connections, with inhibitors')

axis off

function h = plotNeurons(X, Y, Z)
    
    h = zeros(X,Y,Z);
    
    hold on
    
    for x=1:X
        for y=1:Y
            for z=1:Z
                
                h(x,y,z) = plot3(x,y,z, 'b*');
                
            end
        end
    end

end

function changeNeuronPlot(img, handles, internal, activity, restingPotential)

    figure(img)
    
    [Y X Z] = size(handles);
    
    for x=1:X
        for y=1:Y
            for z=1:Z
                
                set(handles(y,x,z), 'LineWidth', internal(y,x,z)*5+0.1 + activity(y,x,z)*2, 'Color', activity(y,x,z)*[0 0 1]);
                
            end
        end
    end
    
        
end

function plotAllSynapses(img, synapses)

    figure(img)
    
    [N m] = size(synapses);
    hold on
    for n = 2:N
        plot3([synapses(n,1) synapses(n,4)] + 1, [synapses(n,2) synapses(n,5)] + 1, [synapses(n,3) synapses(n,6)] + 1, ':k')
    end

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






