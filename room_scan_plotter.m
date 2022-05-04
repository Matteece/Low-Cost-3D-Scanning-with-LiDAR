close all
clear variables

arduino = serialport("COM3",115200);
i = 1;
while 1
    % This is the order of how these variables are sent from the board:
    % radius, phi, theta

    % Check for the "9847" signal sent from Arduino that indicates its done
    % scanning
    temp = readline(arduino);
    temp2 = temp;
    temp = str2double(temp);
    if (temp == 9847)
        break;
    else
        radius(i) = temp2;
    end
    phi(i) = readline(arduino);
    theta(i) = readline(arduino);
    i = i + 1;
end
temp

% Convert input data to integers
radius = str2double(radius);
phi = str2double(phi);
theta = str2double(theta);

% Convert to radians
theta = theta*pi/180;
phi = phi*pi/180;

% Convert coordinates from polar to cartesian
x = radius.*sin(theta).*cos(phi);
y = radius.*sin(theta).*sin(phi);
z = radius.*cos(theta);
% Use pointCloud() for making a 3D point cloud
% First, put xyz points into a 3D matrix
data = cat(3, x, y, z);

cloud = pointCloud(data);
pcshow(cloud)
