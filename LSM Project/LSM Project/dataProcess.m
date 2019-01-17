close all
clear all
clc

filename = 'test.gif';

inputData = load("input.txt");
internalData = load("internal.txt");
activityData = load("activity.txt");

[N, I] = size(inputData);

N = N/I;

input = zeros(I,I,N);
internal = zeros(I,I,N);
activity = zeros(I,I,N);

h = figure;

for n=1:N

    input(1:I,1:I,n) = inputData((n-1)*I+1:n*I,1:I);
   	internal(1:I,1:I,n) = internalData((n-1)*I+1:n*I,1:I);
    activity(1:I,1:I,n) = activityData((n-1)*I+1:n*I,1:I);
    
end

for t = 1:N
    
    subplot(1,3,1)
    image(input(:,:,t))
    title('Input')
    subplot(1,3,2)
    image(internal(:,:,t)*100)
    title('Internal')
    subplot(1,3,3)
    image(activity(:,:,t)*100)
    title('Activations')
    pause(0.1)
    
    %saveToGif(h, t, filename)
    

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






    