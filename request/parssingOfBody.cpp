












int if_has_content_len()
{
    int rtn;
    if(flag_ == 0) // to enter here one time
    {
        rtn = headerOfRequest.find("mp4");
        if(rtn != -1)
            MyFile.open(file.append(index).append(".mp4"));
        rtn = headerOfRequest.find("jpeg");
        if(rtn != -1)
            MyFile.open("file.jpeg");
        rtn = headerOfRequest.find("pdf");
        if(rtn != -1)
            MyFile.open("file.pdf");
        rtn = headerOfRequest.find("application");
        if(rtn != -1)
            MyFile.open("file.txt");// should handle any text file
        flag_ = 1;
    }
    i += len;
    if(i >= ContentLength )// finish recivng
    {
        MyFile << buffer.substr(headerOfRequest.size() + 2,ContentLength);

        MyFile.close();
    }
}







int if_is_chenked()
{
    int dec;
    i = 0;
    while (i < buffer.size())
    {
        if(isalnum(buffer[i]) || isalpha(buffer[i]))
        {
            int k = i;
            while (isalnum(buffer[i]) || isalpha(buffer[i]))
                i++;
            dec = std::stoul(buffer.substr(k,i), NULL, 16);

            i+=2;
    
            if(dec == 0 && flag_ == 0)
            {
                rtn = headerOfRequest.find("mp4");
                if(rtn != -1)
                    MyFile.open(file.append(index).append(".mp4"));
                rtn = headerOfRequest.find("jpeg");
                if(rtn != -1)
                    MyFile.open("file.jpeg");
                rtn = headerOfRequest.find("pdf");
                if(rtn != -1)
                    MyFile.open("file.pdf");
                rtn = headerOfRequest.find("application");
                if(rtn != -1)
                    MyFile.open("file.txt");// should handle any text file
                rtn = 1;
                MyFile <<  bodyofRequest;                     
                flag_ = 10;
            }
            
            while (dec--)
            {
                bodyofRequest.push_back(buffer[i]);
                i++;
            }
            i += 2; 
        }
    }
    // buffer.erase(buffer.begin(),buffer.end());
    buffer.clear();// leaks ?
}