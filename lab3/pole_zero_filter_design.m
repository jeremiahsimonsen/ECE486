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

% Last Modified by GUIDE v2.5 11-Mar-2015 19:47:28

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
handles.pole_freqs = [];
handles.pole_radii = [];
handles.zero_freqs = [];

% Variables for the transfer function
handles.f = 0:.001:.5;
handles.k = 1;

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
handles.pole_freqs(end+1) = handles.new_pole_freq;
handles.pole_radii(end+1) = handles.new_pole_radius;
if mod(handles.new_pole_freq,0.5) ~= 0.0
	handles.poles(end+1) = handles.new_pole_radius*exp(1i*2*pi*handles.new_pole_freq);
	handles.poles(end+1) = handles.new_pole_radius*exp(-1i*2*pi*handles.new_pole_freq);
else
	handles.poles(end+1) = handles.new_pole_radius*exp(1i*2*pi*handles.new_pole_freq);
end
% Update handles structure
guidata(hObject, handles);
% Update Poles table
set(handles.pole_table,'data',[handles.pole_freqs(:), handles.pole_radii(:)]);
% Update pole-zero plot
axes(handles.pole_zero_axes);
zplaneplot(handles.zeros,handles.poles);
% Update Hf plot
Hf = calc_Hf(handles.zeros,handles.poles,handles.f);
update_Hf_plot(handles.f,Hf,handles.Hf_axes);


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


% --- Executes on button press in add_zero_button.
function add_zero_button_Callback(hObject, eventdata, handles)
% hObject    handle to add_zero_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% if ind = find(cellfun('isempty',handles.zero_freqs)==1,1)
% else ind = length(handles.zero_freqs)+1;
% end
handles.zero_freqs(end+1) = handles.new_zero_freq;

if mod(handles.new_zero_freq, 0.5) ~= 0.0
	handles.zeros(end+1) = exp(1i*2*pi*handles.new_zero_freq);
	handles.zeros(end+1) = exp(-1i*2*pi*handles.new_zero_freq);
else
	handles.zeros(end+1) = exp(1i*2*pi*handles.new_zero_freq);
end
% Update handles structure
guidata(hObject, handles);
% Update zero table
set(handles.zero_table,'data',handles.zero_freqs(:));
% Update pole-zero plot
axes(handles.pole_zero_axes);
zplaneplot(handles.zeros,handles.poles);
% Update Hf plot
Hf = calc_Hf(handles.zeros,handles.poles,handles.f);
update_Hf_plot(handles.f,Hf,handles.Hf_axes);


% --- Helper function to calculate the transfer function.
function Hf = calc_Hf(z,poles,f)
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
k = 1/max(abs(Hf));
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
strData = get(hObject,'Data');
data = str2double(strData);
ind = eventdata.Indices(1);
pf = data(isfinite(data(:,1)),1);
pr = data(isfinite(data(:,2)),2);
if length(pf) ~= length(pr)
	return
end
if ind == length(data(:,1))
	strData{end+1,1} = ' ';
	strData{end,2} = ' ';
	set(handles.pole_table,'data',strData);
end
handles.pole_freqs = data(:,1);
handles.pole_radii = data(:,2);
handles.poles = [];
for n=1:length(pf)
	if mod(pf(n),0.5) ~= 0.0
		handles.poles = [handles.poles pr(n)*exp(1i*2*pi*pf(n)) ...
						pr(n)*exp(-1i*2*pi*pf(n))];
	else
		handles.poles = [handles.poles pr(n)*exp(1i*2*pi*pf(n))];
	end
end
% Update handles structure
guidata(hObject, handles);
% Update pole-zero plot
axes(handles.pole_zero_axes);
zplaneplot(handles.zeros,handles.poles);
% Update Hf plot
Hf = calc_Hf(handles.zeros,handles.poles,handles.f);
update_Hf_plot(handles.f,Hf,handles.Hf_axes);


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
strData = get(hObject,'Data');
data = str2double(strData);
ind = eventdata.Indices(1);
if ind == length(data(:,1))
	strData{end+1,1} = ' ';
	set(handles.zero_table,'data',strData);
end
zf = data(isfinite(data(:,1)),1);
handles.zero_freqs = data(:,1);
handles.zeros = [];
for n=1:length(zf)
	if mod(zf(n),0.5) ~= 0.0
		handles.zeros = [handles.zeros exp(1i*2*pi*zf(n)) ...
						exp(-1i*2*pi*zf(n))];
	else
		handles.zeros = [handles.zeros exp(1i*2*pi*zf(n))];
	end
end
% Update handles structure
guidata(hObject, handles);
% Update pole-zero plot
axes(handles.pole_zero_axes);
zplaneplot(handles.zeros,handles.poles);
% Update Hf plot
Hf = calc_Hf(handles.zeros,handles.poles,handles.f);
update_Hf_plot(handles.f,Hf,handles.Hf_axes);
