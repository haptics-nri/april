% Runs apriltag_mex on all PNG files in the current directory:
%
% Inputs: none
% Outputs: names, ids, centers, p1s, p2s, p3s, p4s
%   all cell arrays of the same length
%   names is the filenames, and the rest are the outputs of apriltag_mex
%
% Copyright 2016 Alex Burka
% BSD license
function [names, ids, centers, p1s, p2s, p3s, p4s] = process_all()
    pngs = dir('*.png');
    names = cell(length(pngs), 1);
    ids = cell(length(pngs), 1);
    centers = cell(length(pngs), 1);
    p1s = cell(length(pngs), 1);
    p2s = cell(length(pngs), 1);
    p3s = cell(length(pngs), 1);
    p4s = cell(length(pngs), 1);
    for i=1:length(pngs)
        im = imread(pngs(i).name);
        gray = rgb2gray(im);
        [id, center, p1, p2, p3, p4] = apriltag_mex(gray);
        
        fprintf('(%d/%d) %s: %d tags\n', i, length(pngs), pngs(i).name, length(id));
        
        names{i} = pngs(i).name;
        ids{i} = id;
        centers{i} = center;
        p1s{i} = p1;
        p2s{i} = p2;
        p3s{i} = p3;
        p4s{i} = p4;
    end
end
