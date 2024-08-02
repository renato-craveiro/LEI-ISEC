function [lat, lon] = get_lat_lon_from_google(name)

% Define the name of the place you want to geocode
% name = 'Portugal';

% Create the URL for the geocoding API request
url = sprintf('https://nominatim.openstreetmap.org/search?q=%s&format=json&limit=1', name);

% Send the API request and get the JSON response
options = weboptions('ContentType', 'json');
response = webread(url, options);

% Parse the JSON response and get the latitude and longitude
if ~isempty(response)
    lat = str2double(response.lat);
    lon = str2double(response.lon);
%     fprintf('The latitude of %s is %f\n', name, lat);
%     fprintf('The longitude of %s is %f\n', name, lon);
else
    fprintf('Could not find the location of %s\n', name);
    lat=0;
    lon=0;
end


