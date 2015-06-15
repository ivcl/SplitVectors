function [vectors,colors]=convert(infileName,outfileName)
% convert .data with Boy's surface color

    % read data file
    [infile, msg] = fopen(infileName);
    %infile = fopen('C:\Users\GuohaoZhang\Desktop\Programs\MATLAB\boyssurface\normal_s5.data');
    numTrace = fscanf(infile,'%d',1);
    display(numTrace);
    numSegs = zeros(numTrace,1);
    segEndIdx = zeros(numTrace,1);
    idxCurrent = 0;
    points = zeros(numTrace*100,3);
    for t = 1:numTrace;
        numSegs(t) = fscanf(infile,'%d',1);
        for s = 1:numSegs(t);
            idxCurrent = idxCurrent + 1;
            points(idxCurrent,:)=fscanf(infile,'%f %f %f',3);
            %ignore the rest line
            fgetl(infile);
        end
        segEndIdx(t) = idxCurrent;
    end
    fclose(infile);
    
    % compute directions
    vectors = zeros(numTrace,3);
    startIdx = 1;
    for t = 1:numTrace;
        vectors(t,1) = points(startIdx,1)-points(segEndIdx(t),1);
        vectors(t,2) = points(startIdx,2)-points(segEndIdx(t),2);
        vectors(t,3) = points(startIdx,3)-points(segEndIdx(t),3);
        lgt = norm(vectors(t,:));
        vectors(t,:) = vectors(t,:)/lgt;
        startIdx = segEndIdx(t)+1;
    end
    
    
    % compute colors
    colors = line2rgb(vectors);
    
    % write outfile
    outfile = fopen(outfileName,'w');
    fprintf(outfile,'%d\n',numTrace);
    idxCurrent = 0;
    for t = 1:numTrace;
        fprintf(outfile,'%d\n',numSegs(t));
        for s = 1:numSegs(t);
            idxCurrent = idxCurrent + 1;
            fprintf(outfile,'%f %f %f',points(idxCurrent,1),points(idxCurrent,2),points(idxCurrent,3));
            fprintf(outfile,' %f %f %f \n',colors(t,1),colors(t,2),colors(t,3));
        end
    end
    fclose(outfile);
    
end

