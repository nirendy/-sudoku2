#include "gurobi_c2.h"

int __stdcall
  GRBgetattrinfo(GRBmodel *model, const char *attrname, int *datatypeP,
                 int *sizeP, int *settableP){ return 0;}
int __stdcall
  GRBisattravailable(GRBmodel *model, const char *attrname){ return 0;}
int __stdcall
  GRBgetintattr(GRBmodel *model, const char *attrname, int *valueP){ return 0;}
int __stdcall
  GRBsetintattr(GRBmodel *model, const char *attrname, int newvalue){ return 0;}
int __stdcall
  GRBgetintattrelement(GRBmodel *model, const char *attrname,
                       int element, int *valueP){ return 0;}
int __stdcall
  GRBsetintattrelement(GRBmodel *model, const char *attrname,
                       int element, int newvalue){ return 0;}
int __stdcall
  GRBgetintattrarray(GRBmodel *model, const char *attrname,
                     int first, int len, int *values){ return 0;}
int __stdcall
  GRBsetintattrarray(GRBmodel *model, const char *attrname,
                     int first, int len, int *newvalues){ return 0;}
int __stdcall
  GRBgetintattrlist(GRBmodel *model, const char *attrname,
                    int len, int *ind, int *values){ return 0;}
int __stdcall
  GRBsetintattrlist(GRBmodel *model, const char *attrname,
                    int len, int *ind, int *newvalues){ return 0;}
int __stdcall
  GRBgetcharattrelement(GRBmodel *model, const char *attrname,
                        int element, char *valueP){ return 0;}
int __stdcall
  GRBsetcharattrelement(GRBmodel *model, const char *attrname,
                        int element, char newvalue){ return 0;}
int __stdcall
  GRBgetcharattrarray(GRBmodel *model, const char *attrname,
                      int first, int len, char *values){ return 0;}
int __stdcall
  GRBsetcharattrarray(GRBmodel *model, const char *attrname,
                      int first, int len, char *newvalues){ return 0;}
int __stdcall
  GRBgetcharattrlist(GRBmodel *model, const char *attrname,
                     int len, int *ind, char *values){ return 0;}
int __stdcall
  GRBsetcharattrlist(GRBmodel *model, const char *attrname,
                     int len, int *ind, char *newvalues){ return 0;}
int __stdcall
  GRBgetdblattr(GRBmodel *model, const char *attrname, double *valueP){ return 0;}
int __stdcall
  GRBsetdblattr(GRBmodel *model, const char *attrname, double newvalue){ return 0;}
int __stdcall
  GRBgetdblattrelement(GRBmodel *model, const char *attrname,
                       int element, double *valueP){ return 0;}
int __stdcall
  GRBsetdblattrelement(GRBmodel *model, const char *attrname,
                       int element, double newvalue){ return 0;}
int __stdcall
  GRBgetdblattrarray(GRBmodel *model, const char *attrname,
                     int first, int len, double *values){ return 0;}
int __stdcall
  GRBsetdblattrarray(GRBmodel *model, const char *attrname,
                     int first, int len, double *newvalues){ return 0;}
int __stdcall
  GRBgetdblattrlist(GRBmodel *model, const char *attrname,
                    int len, int *ind, double *values){ return 0;}
int __stdcall
  GRBsetdblattrlist(GRBmodel *model, const char *attrname,
                    int len, int *ind, double *newvalues){ return 0;}
int __stdcall
  GRBgetstrattr(GRBmodel *model, const char *attrname, char **valueP){ return 0;}
int __stdcall
  GRBsetstrattr(GRBmodel *model, const char *attrname, const char *newvalue){ return 0;}
int __stdcall
  GRBgetstrattrelement(GRBmodel *model, const char *attrname,
                       int element, char **valueP){ return 0;}
int __stdcall
  GRBsetstrattrelement(GRBmodel *model, const char *attrname,
                       int element, const char *newvalue){ return 0;}
int __stdcall
  GRBgetstrattrarray(GRBmodel *model, const char *attrname,
                     int first, int len, char **values){ return 0;}
int __stdcall
  GRBsetstrattrarray(GRBmodel *model, const char *attrname,
                     int first, int len, char **newvalues){ return 0;}
int __stdcall
  GRBgetstrattrlist(GRBmodel *model, const char *attrname,
                    int len, int *ind, char **values){ return 0;}
int __stdcall
  GRBsetstrattrlist(GRBmodel *model, const char *attrname,
                    int len, int *ind, char **newvalues){ return 0;}

/* Callback */
int __stdcall
  GRBsetcallbackfunc(GRBmodel *model,
                     int (__stdcall *cb)(CB_ARGS),
                     void  *usrdata){ return 0;}
int __stdcall
  GRBgetcallbackfunc(GRBmodel *model,
                     int (__stdcall **cbP)(CB_ARGS)){ return 0;}
int __stdcall
  GRBsetlogcallbackfunc(GRBmodel *model,
                        int (__stdcall *cb)(char *msg)){ return 0;}
int __stdcall
  GRBsetlogcallbackfuncenv(GRBenv *env,
                           int (__stdcall *cb)(char *msg)){ return 0;}
int __stdcall
  GRBcbget(void *cbdata, int where, int what, void *resultP){ return 0;}
int __stdcall
  GRBcbsetparam(void *cbdata, const char *paramname, const char *newvalue){ return 0;}
int __stdcall
  GRBcbsolution(void *cbdata, const double *solution, double *objvalP){ return 0;}
int __stdcall
  GRBcbcut(void *cbdata, int cutlen, const int *cutind, const double *cutval,
          char cutsense, double cutrhs){ return 0;}
int __stdcall
  GRBcblazy(void *cbdata, int lazylen, const int *lazyind,
            const double *lazyval, char lazysense, double lazyrhs){ return 0;}


/*
   ATTRIBUTES
*/

/* Model attributes */

/* Variable attributes */

/* Constraint attributes */

/* Quadratic constraint attributes */

/* General constraint attributes */

/* Model statistics */

/* Model solution attributes */

/* Variable attributes related to the current solution */

/* Constraint attributes related to the current solution */

/* Solution quality attributes */

/* LP sensitivity analysis */

/* IIS */

/* Tuning */

/* Advanced simplex features */

/* Presolve attribute */

/* Multi objective attribute, controlled by parameter ObjNumber (= i) */

/* General constraints */


/*
   CALLBACKS
*/

/* For callback */

/* Supported names for callback */

int __stdcall
  GRBgetcoeff(GRBmodel *model, int constr, int var, double *valP){ return 0;}
int __stdcall
  GRBgetconstrs(GRBmodel *model, int *numnzP, int *cbeg,
                int *cind, double *cval, int start, int len){ return 0;}
int __stdcall
  GRBXgetconstrs(GRBmodel *model, size_t *numnzP, size_t *cbeg,
                 int *cind, double *cval, int start, int len){ return 0;}
int __stdcall
  GRBgetvars(GRBmodel *model, int *numnzP, int *vbeg, int *vind,
             double *vval, int start, int len){ return 0;}
int __stdcall
  GRBXgetvars(GRBmodel *model, size_t *numnzP, size_t *vbeg, int *vind,
              double *vval, int start, int len){ return 0;}
int __stdcall
  GRBgetsos(GRBmodel *model, int *nummembersP, int *sostype, int *beg,
            int *ind, double *weight, int start, int len){ return 0;}
int __stdcall
  GRBgetgenconstrMax(GRBmodel *model, int genconstr, int *resvarP,
                     int *nvarsP, int *vars, double *constantP){ return 0;}
int __stdcall
  GRBgetgenconstrMin(GRBmodel *model, int genconstr, int *resvarP,
                     int *nvarsP, int *vars, double *constantP){ return 0;}
int __stdcall
  GRBgetgenconstrAbs(GRBmodel *model, int genconstr, int *resvarP, int *argvarP){ return 0;}
int __stdcall
  GRBgetgenconstrAnd(GRBmodel *model, int genconstr, int *resvarP,
                     int *nvarsP, int *vars){ return 0;}
int __stdcall
  GRBgetgenconstrOr(GRBmodel *model, int genconstr, int *resvarP,
                    int *nvarsP, int *vars){ return 0;}
int __stdcall
  GRBgetgenconstrIndicator(GRBmodel *model, int genconstr, int *binvarP, int *binvalP,
                           int *nvarsP, int *vars, double *vals,
                           char *senseP, double *rhsP){ return 0;}
int __stdcall
  GRBgetq(GRBmodel *model, int *numqnzP, int *qrow, int *qcol, double *qval){ return 0;}
int __stdcall
  GRBgetqconstr(GRBmodel *model, int qconstr,
                int *numlnzP, int *lind, double *lval,
                int *numqnzP, int *qrow, int *qcol, double *qval){ return 0;}
int __stdcall
  GRBgetvarbyname(GRBmodel *model, const char *name, int *indexP){ return 0;}
int __stdcall
  GRBgetconstrbyname(GRBmodel *model, const char *name, int *indexP){ return 0;}
int __stdcall
  GRBgetpwlobj(GRBmodel *model, int var, int *pointsP,
               double *x, double *y){ return 0;}

int __stdcall
  GRBoptimize(GRBmodel *model){ return 0;}
int __stdcall
  GRBoptimizeasync(GRBmodel *model){ return 0;}

GRBmodel * __stdcall
  GRBcopymodel(GRBmodel *model){ return 0;}
GRBmodel * __stdcall
  GRBfixedmodel(GRBmodel *model){ return 0;}
int __stdcall
  GRBfeasrelax(GRBmodel *model, int relaxobjtype, int minrelax,
               double *lbpen, double *ubpen, double *rhspen,
               double *feasobjP){ return 0;}

/* Undocumented routines */

int __stdcall
  GRBgetcbwhatinfo(void *cbdata, int what, int *typeP, int *sizeP){ return 0;}
GRBmodel * __stdcall
  GRBrelaxmodel(GRBmodel *model){ return 0;}
int __stdcall
  GRBconverttofixed(GRBmodel *model){ return 0;}
GRBmodel * __stdcall
  GRBpresolvemodel(GRBmodel *model){ return 0;}
GRBmodel * __stdcall
  GRBiismodel(GRBmodel *model){ return 0;}
GRBmodel * __stdcall
  GRBfeasibility(GRBmodel *model){ return 0;}
GRBmodel * __stdcall
  GRBlinearizemodel(GRBmodel *model){ return 0;}

int __stdcall
  GRBloadenvsyscb(GRBenv **envP, const char *logfilename,
                  void * (__stdcall *malloccb)(MALLOCCB_ARGS),
                  void * (__stdcall *calloccb)(CALLOCCB_ARGS),
                  void * (__stdcall *realloccb)(REALLOCCB_ARGS),
                  void (__stdcall *freecb)(FREECB_ARGS),
                  int (__stdcall *threadcreatecb)(THREADCREATECB_ARGS),
                  void (__stdcall *threadjoincb)(THREADJOINCB_ARGS),
                  void *syscbusrdata){ return 0;}
/* Copyright 2016, Gurobi Optimization, Inc. */

int __stdcall
  GRBreadmodel(GRBenv *env, const char *filename, GRBmodel **modelP){ return 0;}
int __stdcall
  GRBread(GRBmodel *model, const char *filename){ return 0;}
int __stdcall
  GRBwrite(GRBmodel *model, const char *filename){ return 0;}
int __stdcall
  GRBismodelfile(const char *filename){ return 0;}

int __stdcall
  GRBnewmodel(GRBenv *env, GRBmodel **modelP, const char *Pname, int numvars,
              double *obj, double *lb, double *ub, char *vtype,
              char **varnames){ return 0;}

int __stdcall
  GRBloadmodel(GRBenv *env, GRBmodel **modelP, const char *Pname,
               int numvars, int numconstrs,
               int objsense, double objcon, double *obj,
               char *sense, double *rhs,
               int *vbeg, int *vlen, int *vind, double *vval,
               double *lb, double *ub, char *vtype,
               char **varnames, char **constrnames){ return 0;}

int __stdcall
  GRBXloadmodel(GRBenv *env, GRBmodel **modelP, const char *Pname,
                int numvars, int numconstrs,
                int objsense, double objcon, double *obj,
                char *sense, double *rhs,
                size_t *vbeg, int *vlen, int *vind, double *vval,
                double *lb, double *ub, char *vtype,
                char **varnames, char **constrnames){ return 0;}

int __stdcall
  GRBaddvar(GRBmodel *model, int numnz, int *vind, double *vval,
            double obj, double lb, double ub, char vtype,
            const char *varname){ return 0;}
int __stdcall
  GRBaddvars(GRBmodel *model, int numvars, int numnz,
             int *vbeg, int *vind, double *vval,
             double *obj, double *lb, double *ub, char *vtype,
             char **varnames){ return 0;}
int __stdcall
  GRBXaddvars(GRBmodel *model, int numvars, size_t numnz,
              size_t *vbeg, int *vind, double *vval,
              double *obj, double *lb, double *ub, char *vtype,
              char **varnames){ return 0;}
int __stdcall
  GRBaddconstr(GRBmodel *model, int numnz, int *cind, double *cval,
               char sense, double rhs, const char *constrname){ return 0;}
int __stdcall
  GRBaddconstrs(GRBmodel *model, int numconstrs, int numnz,
                int *cbeg, int *cind, double *cval,
                char *sense, double *rhs, char **constrnames){ return 0;}
int __stdcall
  GRBXaddconstrs(GRBmodel *model, int numconstrs, size_t numnz,
                 size_t *cbeg, int *cind, double *cval,
                 char *sense, double *rhs, char **constrnames){ return 0;}
int __stdcall
  GRBaddrangeconstr(GRBmodel *model, int numnz, int *cind, double *cval,
                    double lower, double upper, const char *constrname){ return 0;}
int __stdcall
  GRBaddrangeconstrs(GRBmodel *model, int numconstrs, int numnz,
                     int *cbeg, int *cind, double *cval,
                     double *lower, double *upper, char **constrnames){ return 0;}
int __stdcall
  GRBXaddrangeconstrs(GRBmodel *model, int numconstrs, size_t numnz,
                      size_t *cbeg, int *cind, double *cval,
                      double *lower, double *upper, char **constrnames){ return 0;}
int __stdcall
  GRBaddsos(GRBmodel *model, int numsos, int nummembers, int *types,
            int *beg, int *ind, double *weight){ return 0;}
int __stdcall
  GRBaddgenconstrMax(GRBmodel *model, const char *name,
                     int resvar, int nvars, const int *vars,
                     double constant){ return 0;}
int __stdcall
  GRBaddgenconstrMin(GRBmodel *model, const char *name,
                     int resvar, int nvars, const int *vars,
                     double constant){ return 0;}
int __stdcall
  GRBaddgenconstrAbs(GRBmodel *model, const char *name,
                     int resvar, int argvar){ return 0;}
int __stdcall
  GRBaddgenconstrAnd(GRBmodel *model, const char *name,
                     int resvar, int nvars, const int *vars){ return 0;}
int __stdcall
  GRBaddgenconstrOr(GRBmodel *model, const char *name,
                    int resvar, int nvars, const int *vars){ return 0;}
int __stdcall
  GRBaddgenconstrIndicator(GRBmodel *lp, const char *name,
                           int binvar, int binval, int nvars, const int *vars,
                           const double *vals, char sense, double rhs){ return 0;}
int __stdcall
  GRBaddqconstr(GRBmodel *model, int numlnz, int *lind, double *lval,
                int numqnz, int *qrow, int *qcol, double *qval,
                char sense, double rhs, const char *QCname){ return 0;}
int __stdcall
  GRBaddcone(GRBmodel *model, int nummembers, int *members){ return 0;}
int __stdcall
  GRBaddqpterms(GRBmodel *model, int numqnz, int *qrow, int *qcol,
                double *qval){ return 0;}
int __stdcall
  GRBdelvars(GRBmodel *model, int len, int *ind){ return 0;}
int __stdcall
  GRBdelconstrs(GRBmodel *model, int len, int *ind){ return 0;}
int __stdcall
  GRBdelsos(GRBmodel *model, int len, int *ind){ return 0;}
int __stdcall
  GRBdelgenconstrs(GRBmodel *model, int len, int *ind){ return 0;}
int __stdcall
  GRBdelqconstrs(GRBmodel *model, int len, int *ind){ return 0;}
int __stdcall
  GRBdelq(GRBmodel *model){ return 0;}
int __stdcall
  GRBchgcoeffs(GRBmodel *model, int cnt, int *cind, int *vind, double *val){ return 0;}
int __stdcall
  GRBXchgcoeffs(GRBmodel *model, size_t cnt, int *cind, int *vind, double *val){ return 0;}
int __stdcall
  GRBsetpwlobj(GRBmodel *model, int var, int points, double *x,
               double *y){ return 0;}

int __stdcall
  GRBupdatemodel(GRBmodel *model){ return 0;}

int __stdcall
  GRBresetmodel(GRBmodel *model){ return 0;}

int __stdcall
  GRBfreemodel(GRBmodel *model){ return 0;}

int __stdcall
  GRBcomputeIIS(GRBmodel *model){ return 0;}

/* simplex advanced routines */


int __stdcall
  GRBFSolve(GRBmodel *model, GRBsvec *b, GRBsvec *x){ return 0;}

int __stdcall
  GRBBinvColj(GRBmodel *model, int j, GRBsvec *x){ return 0;}

int __stdcall
  GRBBinvj(GRBmodel *model, int j, GRBsvec *x){ return 0;}

int __stdcall
  GRBBSolve(GRBmodel *model, GRBsvec *b, GRBsvec *x){ return 0;}

int __stdcall
  GRBBinvi(GRBmodel *model, int i, GRBsvec *x){ return 0;}

int __stdcall
  GRBBinvRowi(GRBmodel *model, int i, GRBsvec *x){ return 0;}

int __stdcall
  GRBgetBasisHead(GRBmodel *model, int *bhead){ return 0;}

/* Model status codes (after call to GRBoptimize()) */

/* Basis status info */

/* Cpyright 2016, Gurobi Optimization, Inc. */

/* Undocumented routines */

int __stdcall
  GRBstrongbranch(GRBmodel *model, int num, int *cand,
                  double *downobjbd, double *upobjbd, int *statusP){ return 0;}
/* Copyright 2016, Gurobi Optimization, Inc. */

/**************/
/* Parameters */
/**************/

/* Termination */

/* Tolerances */

/* Simplex */

/* Barrier */

/* MIP */

/* MIP cuts */

/* Distributed algorithms */

/* Other */

/* Parameter enumerations */

/* All *CUTS parameters */
int __stdcall
  GRBcheckmodel(GRBmodel *model){ return 0;}
void __stdcall
  GRBsetsignal(GRBmodel *model){ }
void __stdcall
  GRBterminate(GRBmodel *model){ }
int __stdcall
  GRBreplay(const char *filename){ return 0;}
void __stdcall
  GRBclean2(int *lenP, int *ind, double *val){ }
void __stdcall
  GRBclean3(int *lenP, int *ind0, int *ind1, double *val){ }

/* Logging */

void __stdcall
  GRBmsg(GRBenv *env, const char *message){ }


/* The following four routines are deprecated in Gurobi -2.0.
   Use the 'LogFile' parameter to control logging instead. */

int __stdcall
  GRBgetlogfile(GRBenv *env, FILE **logfileP){ return 0;}
int __stdcall
  GRBsetlogfile(GRBenv *env, FILE *logfile){ return 0;}


/* Parameter routines */

int __stdcall
  GRBgetintparam(GRBenv *env, const char *paramname, int *valueP){ return 0;}
int __stdcall
  GRBgetdblparam(GRBenv *env, const char *paramname, double *valueP){ return 0;}
int __stdcall
  GRBgetstrparam(GRBenv *env, const char *paramname, char *valueP){ return 0;}
int __stdcall
  GRBgetintparaminfo(GRBenv *env, const char *paramname, int *valueP,
                     int *minP, int *maxP, int *defP){ return 0;}
int __stdcall
  GRBgetdblparaminfo(GRBenv *env, const char *paramname, double *valueP,
                     double *minP, double *maxP, double *defP){ return 0;}
int __stdcall
  GRBgetstrparaminfo(GRBenv *env, const char *paramname, char *valueP,
                     char *defP){ return 0;}
int __stdcall
  GRBsetparam(GRBenv *env, const char *paramname, const char *value){ return 0;}
int __stdcall
  GRBsetintparam(GRBenv *env, const char *paramname, int value){ return 0;}
int __stdcall
  GRBsetdblparam(GRBenv *env, const char *paramname, double value){ return 0;}
int __stdcall
  GRBsetstrparam(GRBenv *env, const char *paramname, const char *value){ return 0;}
int __stdcall
  GRBgetparamtype(GRBenv *env, const char *paramname){ return 0;}
int __stdcall
  GRBresetparams(GRBenv *env){ return 0;}
int __stdcall
  GRBcopyparams(GRBenv *dest, GRBenv *src){ return 0;}
int __stdcall
  GRBwriteparams(GRBenv *env, const char *filename){ return 0;}
int __stdcall
  GRBreadparams(GRBenv *env, const char *filename){ return 0;}
int __stdcall
  GRBgetnumparams(GRBenv *env){ return 0;}
int __stdcall
  GRBgetparamname(GRBenv *env, int i, char **paramnameP){ return 0;}
int __stdcall
  GRBgetnumattributes(GRBmodel *model){ return 0;}
int __stdcall
  GRBgetattrname(GRBmodel *model, int i, char **attrnameP){ return 0;}

/* Environment routines */

int __stdcall
  GRBloadenv(GRBenv **envP, const char *logfilename){ return 0;}
int __stdcall
  GRBloadenvadv(GRBenv **envP, const char *logfilename,
                int apitype, int major, int minor, int tech,
                int (__stdcall *cb)(CB_ARGS), void *usrdata){ return 0;}
int __stdcall
  GRBloadclientenv(GRBenv **envP, const char *logfilename,
                   const char *computeservers, int port, const char *password,
                   int priority, double timeout){ return 0;}
int __stdcall
  GRBloadclientenvadv(GRBenv **envP, const char *logfilename,
                      const char *computeservers, int port,
                      const char *password, int priority, double timeout,
                      int apitype, int major, int minor, int tech,
                      int (__stdcall *cb)(CB_ARGS), void *usrdata){ return 0;}
int __stdcall
  GRBloadcloudenv(GRBenv **envP, const char *logfilename,
                  const char *accessID, const char *secretKey,
                  const char *pool){ return 0;}
int __stdcall
  GRBloadcloudenvadv(GRBenv **envP, const char *logfilename,
                     const char *accessID, const char *secretKey,
                     const char *pool, int apitype, int major,
                     int minor, int tech,
                     int (__stdcall *cb)(CB_ARGS), void *usrdata){ return 0;}
GRBenv *__stdcall
  GRBgetenv(GRBmodel *model){ return 0;}
GRBenv *__stdcall
  GRBgetconcurrentenv(GRBmodel *model, int num){ return 0;}
void __stdcall
  GRBdiscardconcurrentenvs(GRBmodel *model){ }
void __stdcall
  GRBreleaselicense(GRBenv *env){ }
void __stdcall
  GRBfreeenv(GRBenv *env){ }
const char * __stdcall
  GRBgeterrormsg(GRBenv *env){ return 0;}
const char * __stdcall
  GRBgetmerrormsg(GRBmodel *model){ return 0;}

/* Version info */

void __stdcall
  GRBversion(int *majorP, int *minorP, int *technicalP){ }

char * __stdcall
  GRBplatform(void){ return 0;}

int __stdcall
  GRBlisttokens(void){ return 0;}
int __stdcall
  GRBlistclients(const char *computeServer, int port){ return 0;}
int __stdcall
  GRBchangeuserpassword(const char *computeServer, int port,
                        const char *admin_password,
                        const char *new_user_password){ return 0;}
int __stdcall
  GRBchangeadminpassword(const char *computeServer, int port,
                         const char *admin_password,
                         const char *new_admin_password){ return 0;}
int __stdcall
  GRBchangejoblimit(const char *computeServer, int port, int newlimit,
                    const char *admin_password){ return 0;}
int __stdcall
  GRBkilljob(const char *computeServer, int port, const char *jobID,
             const char *admin_password){ return 0;}
int __stdcall
  GRBshutdown(const char *computeServer, int port, const char *admin_password){ return 0;}

/* Tuning */

int __stdcall
  GRBtunemodel(GRBmodel *model){ return 0;}
int __stdcall
  GRBtunemodels(int nummodels, GRBmodel **models,
                GRBmodel *ignore, GRBmodel *hint){ return 0;}
int __stdcall
  GRBgettuneresult(GRBmodel *model, int i){ return 0;}
int __stdcall
  GRBgettunelog(GRBmodel *model, int i, char **logP){ return 0;}
int __stdcall
  GRBtunemodeladv(GRBmodel *model, GRBmodel *ignore, GRBmodel *hint){ return 0;}
/* Copyright 2016, Gurobi Optimization, Inc. */

/* Async interface */

int __stdcall
  GRBsync(GRBmodel *model){ return 0;}
