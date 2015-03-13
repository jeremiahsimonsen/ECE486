function varargout = pole_zero_filter_design(varargin)
% POLE_ZERO_FILTER_DESIGN MATLAB code for pole_zero_filter_design.fig
%      POLE_ZERO_FILTER_DESIGN, by itself, creates a new POLE_ZERO_FILTER_DESIGN or raises the existing
%      singleton*.
%
%      H = POLE_ZERO_FILTER_DESIGN returns the handle to a new POLE_ZERO_FILTER_DESIGN or the handle to
%      the existing singleton*.
%
%      POLE_ZERO_FILTER_DESIGN('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in POLE_ZERO_FILTER_DESIGN.M with the given input arguments.
%
%      POLE_ZERO_FILTER_DESIGN('Property','Value',...) creates a new POLE_ZERO_FILTER_DESIGN or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before pole_zero_filter_design_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to pole_zero_filter_design_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help pole_zero_filter_design

% Last Modified by GUIDE v2.5 13-Mar-2015 00:07:59

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @pole_zero_filter_design_OpeningFcn, ...
                   'gui_OutputFcn',  @pole_zero_filter_design_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before pole_zero_filter_design is made visible.
function pole_zero_filter_design_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to pole_zero_filter_design (see VARARGIN)
handles.poles = [];
handles.zeros = [];
% Variables updated by text edit boxes
handles.new_pole_freq = 0.0;
handles.new_pole_radius = 0.0;
handles.new_zero_freq = 0.0;

% Variables to store pole frequencies and radii and zero frequencies; these
% variables go in the tables
handles.pole_freqs = {[];[];[];[]};
handles.pole_radii = {[];[];[];[]};
handles.zero_freqs = {[];[];[];[]};

% Variables for the transfer function
handles.f = 0:.001:.5;
handles.gain_adj = 0;

% Initialize pole-zero plot
axes(handles.pole_zero_axes);
zplaneplot([],[]);

% Initialize the Hf plot
axes(handles.Hf_axes);
plot(handles.f,0);
% Choose default command line output for pole_zero_filter_design
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes pole_zero_filter_design wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = pole_zero_filter_design_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function pole_freq_Callback(hObject, eventdata, handles)
% hObject    handle to pole_freq (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.new_pole_freq = str2double(get(hObject,'string'));
if isnan(handles.new_pole_freq)
	errordlg('You must enter a numeric value','Invalid Input','modal')
	uicontrol(hObject)
	return
end
% Update handles structure
guidata(hObject, handles);
% Hints: get(hObject,'String') returns contents of pole_freq as text
%        str2double(get(hObject,'String')) returns contents of pole_freq as a double


% --- Executes during object creation, after setting all properties.
function pole_freq_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pole_freq (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function pole_radius_Callback(hObject, eventdata, handles)
% hObject    handle to pole_radius (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.new_pole_radius = str2double(get(hObject,'string'));
if isnan(handles.new_pole_radius)
	errordlg('You must enter a numeric value','Invalid Input','modal')
	uicontrol(hObject)
	return
end
% Update handles structure
guidata(hObject, handles);
% Hints: get(hObject,'String') returns contents of pole_radius as text
%        str2double(get(hObject,'String')) returns contents of pole_radius as a double


% --- Executes during object creation, after setting all properties.
function pole_radius_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pole_radius (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in add_pole_button.
function add_pole_button_Callback(hObject, eventdata, handles)
% hObject    handle to add_pole_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Find the first empty row
empty1 = cellfun('isempty',handles.pole_freqs);
empty2 = cellfun('isempty',handles.pole_radii);
tmp = find(empty1&empty2,1);
last = length(handles.pole_freqs);	% the last row
if tmp				% there is an empty row
	ind = tmp;		% set index to first empty row
	% If the first empty row is the last row, add a row so that the user
	% can add a pole by directly editing the table still
	if ind == last
		handles.pole_freqs{ind+1} = [];		% could probably use [], not ''
		handles.pole_radii{ind+1} = [];
	end
% If there are no empty rows (should never actually occur), set index to
% one beyond the last row and add an empty row after the new row
else
	ind = last+1;
	handles.pole_freqs{ind+1} = [];	
	handles.pole_radii{ind+1} = [];
end
% Add new pole frequency and radius to respective cell arrays
handles.pole_freqs{ind} = handles.new_pole_freq;
handles.pole_radii{ind} = handles.new_pole_radius;

% Calculate the new pole z-coordinates
% If the new pole is not on the unit circle, add conjugate pole
if mod(handles.new_pole_freq,0.5) ~= 0.0
	handles.poles(end+1) = handles.new_pole_radius*exp(1i*2*pi*handles.new_pole_freq);
	handles.poles(end+1) = handles.new_pole_radius*exp(-1i*2*pi*handles.new_pole_freq);
% On the unit circle, don't need to add conjugate pole
else
	handles.poles(end+1) = handles.new_pole_radius*exp(1i*2*pi*handles.new_pole_freq);
end
% Update Poles table
set(handles.pole_table,'data',[handles.pole_freqs(:), handles.pole_radii(:)]);
% Update pole-zero plot
axes(handles.pole_zero_axes);
zplaneplot(handles.zeros,handles.poles);
% Update Hf plot
[Hf,g] = calc_Hf(handles.zeros,handles.poles,handles.f,handles.gain_adj);
set(handles.gain_factor,'String',num2str(g));
update_Hf_plot(handles.f,Hf,handles.Hf_axes);
% Update handles structure
guidata(hObject, handles);


% --- Executes on button press in add_zero_button.
function add_zero_button_Callback(hObject, eventdata, handles)
% hObject    handle to add_zero_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Find the first empty cell
tmp = find(cellfun('isempty',handles.zero_freqs)==1,1);
last = length(handles.zero_freqs);		% index of last cell
if tmp				% there is an empty cell
	ind = tmp;		% set index to first empty cell
	% If the first empty cell is the last cell, add a cell so that the user
	% can add a zero by directly editing the table still
	if ind == last
		handles.zero_freqs{ind+1} = [];
	end
else				% there are no empty cells (should never actually occur)
	ind = last+1;	% set index to one beyond last cell
	handles.zero_freqs{ind+1} = '';	% add an empty cell after the new one
end
% add the new zero frequency to the zero_freqs cell array
handles.zero_freqs{ind} = handles.new_zero_freq;

% Calculate z-coordinates of the new zero
% If the new zero is not on the real axis add conjugate zeros so that the
% filter coefficients will come out real. Otherwise we can just add a
% single zero on the real axis
if mod(handles.new_zero_freq, 0.5) ~= 0.0
	handles.zeros(end+1) = exp(1i*2*pi*handles.new_zero_freq);
	handles.zeros(end+1) = exp(-1i*2*pi*handles.new_zero_freq);
else
	handles.zeros(end+1) = exp(1i*2*pi*handles.new_zero_freq);
end
% Update zero table
set(handles.zero_table,'data',handles.zero_freqs(:));
% Update pole-zero plot
axes(handles.pole_zero_axes);
zplaneplot(handles.zeros,handles.poles);
% Update Hf plot
[Hf,g] = calc_Hf(handles.zeros,handles.poles,handles.f,handles.gain_adj);
set(handles.gain_factor,'String',num2str(g));
update_Hf_plot(handles.f,Hf,handles.Hf_axes);
% Update handles structure
guidata(hObject, handles);



function zero_freq_Callback(hObject, eventdata, handles)
% hObject    handle to zero_freq (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.new_zero_freq = str2double(get(hObject,'string'));
if isnan(handles.new_zero_freq)
	errordlg('You must enter a numeric value','Invalid Input','modal')
	uicontrol(hObject)
	return
end
% Update handles structure
guidata(hObject, handles);
% Hints: get(hObject,'String') returns contents of zero_freq as text
%        str2double(get(hObject,'String')) returns contents of zero_freq as a double


% --- Executes during object creation, after setting all properties.
function zero_freq_CreateFcn(hObject, eventdata, handles)
% hObject    handle to zero_freq (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Helper function to calculate the transfer function.
function [Hf,k] = calc_Hf(z,poles,f,adj)
a = poly(poles);
b = poly(z);
num = zeros(1, length(f));
den = zeros(1, length(f));
for n=1:length(a)
	den = den + a(n)*exp(-1i*2*pi*(n-1).*f);
end
for n=1:length(b)
	num = num + b(n)*exp(-1i*2*pi*(n-1).*f);
end
Hf = num ./ den;
k = 1/max(abs(Hf))+adj;
Hf = k .* Hf;


% --- Helper function to update the Hf plot
function update_Hf_plot(f, Hf, ax)
axes(ax);
% TODO: add patches code
plot(f, 20*log10(abs(Hf))); grid on;
axis([0 0.5 -45 5]);
xlabel('Frequency (f, cycles/sample)');
ylabel('Filter Gain (dB)');


% --- Executes when entered data in editable cell(s) in pole_table.
function pole_table_CellEditCallback(hObject, eventdata, handles)
% hObject    handle to pole_table (see GCBO)
% eventdata  structure with the following fields (see UITABLE)
%	Indices: row and column indices of the cell(s) edited
%	PreviousData: previous data for the cell(s) edited
%	EditData: string(s) entered by the user
%	NewData: EditData or its converted form set on the Data property. Empty if Data was not changed
%	Error: error string when failed to convert EditData to appropriate value for Data
% handles    structure with handles and user data (see GUIDATA)

% Make sure that user entered a valid number
if isnan(str2double(eventdata.EditData))
	errordlg('You must enter a numeric value','Invalid Input','modal')
	return
end

cellData = get(hObject,'Data');		% Fetch the data in the table
% Make sure evey element in the table is a number, not a string
for row=1:length(cellData(:,1))
	for col = 1:length(cellData(1,:))
		if ischar(cellData{row,col})	% element is a string
			cellData{row,col} = str2double(cellData(row,col));	% make #
% 			cellData{row,col} = str2double(cellData(row,col));
		end
	end
end
% cellData = str2double(cellData);
% Update global pole frequencies and pole radii
handles.pole_freqs = cellData(:,1);
handles.pole_radii = cellData(:,2);

% If the edited cell was in the last row, add an empty row and update the
% table display data
ind = eventdata.Indices(1);			% row of edited cell
last = length(cellData(:,1));		% index of last row
if ind == last						% if last row - table full
	cellData(ind+1,1) = [];			% add an empty row
	cellData(ind+1,2) = [];
	set(handles.pole_table,'data',cellData);
end

% Recalculate the z-coordinates of all the poles. This is necessary in case
% the user removes/edits a zero in the table
pf = cell2mat(handles.pole_freqs);	% convert to regular array
pr = cell2mat(handles.pole_radii);	% convert to regular array
% pf = handles.pole_freqs;
% pr = handles.pole_radii;
pf = pf(isfinite(pf));				% get rid of NaNs
pr = pr(isfinite(pr));
if length(pf) ~= length(pr)
	return
end
handles.poles = [];					% Empty the poles z-coordinate array
for n=1:length(pf)					% For every pole frequency
	% If not on the unit circle, add conjugate poles to make the filter
	% coefficients come out real
	if mod(pf(n),0.5) ~= 0.0
		handles.poles = [handles.poles pr(n)*exp(1i*2*pi*pf(n)) ...
						pr(n)*exp(-1i*2*pi*pf(n))];
	% On the unit circle, no need to add conjugate pole
	else
		handles.poles = [handles.poles pr(n)*exp(1i*2*pi*pf(n))];
	end
end

% Update pole-zero plot
axes(handles.pole_zero_axes);
zplaneplot(handles.zeros,handles.poles);
% Update Hf plot
[Hf,g] = calc_Hf(handles.zeros,handles.poles,handles.f,handles.gain_adj);
set(handles.gain_factor,'String',num2str(g));
update_Hf_plot(handles.f,Hf,handles.Hf_axes);
% Update handles structure
guidata(hObject, handles);


% --- Executes when entered data in editable cell(s) in zero_table.
function zero_table_CellEditCallback(hObject, eventdata, handles)
% hObject    handle to zero_table (see GCBO)
% eventdata  structure with the following fields (see UITABLE)
%	Indices: row and column indices of the cell(s) edited
%	PreviousData: previous data for the cell(s) edited
%	EditData: string(s) entered by the user
%	NewData: EditData or its converted form set on the Data property. Empty if Data was not changed
%	Error: error string when failed to convert EditData to appropriate value for Data
% handles    structure with handles and user data (see GUIDATA)

% Make sure that user entered a valid number
if isnan(str2double(eventdata.EditData))
	errordlg('You must enter a numeric value','Invalid Input','modal')
	return
end

cellData = get(hObject,'Data');		% Fetch the data in the table
% Make sure every element in the table is a number, not a string
for n=1:length(cellData)
	if ischar(cellData{n})			% if element is a string
		cellData{n} = str2double(cellData{n});	% make it a number
	end
end
% Update global zero frequencies; set explicitly to first column for
% robustness - if the first zero is added from the table, strData can be a
% 2d cell array
handles.zero_freqs = cellData(:,1);

% If the edited cell was the last cell, add an empty cell and update the
% table display data
ind = eventdata.Indices(1);			% row of edited cell
last = length(cellData);			% index of last cell
if ind == last						% if last cell - table full
	cellData{end+1,1} = [];			% add an empty cell
	set(handles.zero_table,'data',cellData);	% update table data
end

% Now we are going to recalculate the z-coordinates of all of our zeros.
% This is necessary in case a user removes/edits a zero in the table
zf = cell2mat(handles.zero_freqs);	% convert to regular array
zf = zf(isfinite(zf));				% get rid of NaNs
handles.zeros = [];					% Empty the zeros z-coordinate array
for n=1:length(zf)					% For every zero frequency
	% If not on the unit circle, add conjugate zeros to make the filter
	% coefficients come out real
	if mod(zf(n),0.5) ~= 0.0
		handles.zeros = [handles.zeros exp(1i*2*pi*zf(n)) ...
						exp(-1i*2*pi*zf(n))];
	% On the unit circle, no need to add conjugate zero
	else
		handles.zeros = [handles.zeros exp(1i*2*pi*zf(n))];
	end
end

% Update pole-zero plot
axes(handles.pole_zero_axes);		% make pole-zero plot axes active
zplaneplot(handles.zeros,handles.poles);	% generate the plot
% Update Hf plot
[Hf,g] = calc_Hf(handles.zeros,handles.poles,handles.f,handles.gain_adj);% evaluate H(f)
set(handles.gain_factor,'String',num2str(g));
update_Hf_plot(handles.f,Hf,handles.Hf_axes);			% plot the tf
% Update handles structure
guidata(hObject, handles);



function gain_adjust_Callback(hObject, eventdata, handles)
% hObject    handle to gain_adjust (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.gain_adj = str2double(get(hObject,'string'));
if isnan(handles.gain_adj)
	errordlg('You must enter a numeric value','Invalid Input','modal')
	uicontrol(hObject)
	return
end
% Update Hf plot
[Hf,g] = calc_Hf(handles.zeros,handles.poles,handles.f,handles.gain_adj);% evaluate H(f)
set(handles.gain_factor,'String',num2str(g));
update_Hf_plot(handles.f,Hf,handles.Hf_axes);			% plot the tf
% Update handles structure
guidata(hObject, handles);
% Hints: get(hObject,'String') returns contents of gain_adjust as text
%        str2double(get(hObject,'String')) returns contents of gain_adjust as a double


% --- Executes during object creation, after setting all properties.
function gain_adjust_CreateFcn(hObject, eventdata, handles)
% hObject    handle to gain_adjust (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
