#include "CGI.hpp"

CGI::~CGI(){ close(cgi_pipe_[0]); }

CGI::CGI(){}

void  CGI::forkCGI(std::string script_name, Kqueue* kq_ptr, int clnt_ident){
  script_name_ = script_name;
  kq_ptr_ = kq_ptr;
  clnt_ident_ = clnt_ident;
  if (pipe(cgi_pipe_) == -1)
    throw (std::runtime_error("pipe() Error"));
  close(cgi_pipe_[0]);
  kq_ptr_->ChangeEvent(clnt_ident_, EVFILT_READ, EV_ADD | EV_ENABLE, NULL);
  cgi_pid_ = fork();
  if (cgi_pid_ == -1){
    close(cgi_pipe_[1]);
    throw (std::runtime_error("fork() Error"));
  }
  kq_ptr_->ChangeEvent(cgi_pid_, EVFILT_PROC, NOTE_EXIT, NULL);
  if (cgi_pid_){
    close(cgi_pipe_[0]);
    // dup2(cgi_pipe_[1], 1);
    // close(cgi_pipe_[1]);
    char* exec_file[3];
    exec_file[0] = (char*)"/usr/local/bin/python3";
    exec_file[1] = script_name_.c_str();
    exec_file[2] = NULL;
    if (execve(exec_file[0], exec_file, NULL) == -1)//envp needed
      throw (std::runtime_error("execve() Error"));
  }
  else
    close(cgi_pipe_[1]);
}
